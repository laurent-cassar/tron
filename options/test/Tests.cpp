// Tests pour Vector3 / Vector4 / Matrix4x4 / Transform / Camera.
// Exécutable console autonome, pas besoin de fenêtre ni de device Direct3D
// (Transform/Camera ne font que des calculs DirectXMath).
//
// Compilation :
//   g++ Tests.cpp Camera.cpp Transform.cpp -o Tests.exe -std=c++17
//
// Execution :
//   Tests.exe

#include "TestFramework.h"
#include "Camera.h"
#include "Quaternion.h"

using namespace DirectX; // pour XM_PIDIV2, XM_2PI, ...

void TestVector3()
{
    SECTION("Vector3");

    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);

    CHECK_VEC3_EQ(a + b, Vector3(5.0f, 7.0f, 9.0f), "Addition (1,2,3)+(4,5,6) = (5,7,9)");
    CHECK_VEC3_EQ(b - a, Vector3(3.0f, 3.0f, 3.0f), "Soustraction (4,5,6)-(1,2,3) = (3,3,3)");
    CHECK_VEC3_EQ(a * 2.0f, Vector3(2.0f, 4.0f, 6.0f), "Multiplication scalaire (1,2,3)*2 = (2,4,6)");

    CHECK_FLOAT_EQ(Vector3(3.0f, 4.0f, 0.0f).Length(), 5.0f, "Length((3,4,0)) == 5 (triangle 3-4-5)");
    CHECK_FLOAT_EQ(Vector3(1.0f, 0.0f, 0.0f).Normalized().Length(), 1.0f, "Un vecteur normalisé a une longueur de 1");

    CHECK_FLOAT_EQ(Vector3::Dot(Vector3::Right(), Vector3::Up()), 0.0f, "Right et Up sont orthogonaux (dot == 0)");
    CHECK_FLOAT_EQ(Vector3::Dot(Vector3::Forward(), Vector3::Forward()), 1.0f, "Dot(Forward, Forward) == 1");

    CHECK_VEC3_EQ(Vector3::Cross(Vector3::Right(), Vector3::Up()), Vector3::Forward(),
                  "Cross(Right, Up) == Forward (repère main gauche)");
}

void TestVector4()
{
    SECTION("Vector4 (quaternions)");

    Vector4 identity = Vector4::Identity();
    CHECK(ApproxEqual(identity.w, 1.0f) && ApproxEqual(identity.x, 0.0f) && ApproxEqual(identity.y, 0.0f) && ApproxEqual(identity.z, 0.0f),
          "Quaternion identité == (0,0,0,1)");

    Vector4 noRotation = Vector4::FromEuler(0.0f, 0.0f, 0.0f);
    CHECK(ApproxEqual(noRotation.w, 1.0f), "FromEuler(0,0,0) donne un quaternion identité (w == 1)");

    Vector4 axisAngleZero = Vector4::FromAxisAngle(Vector3::Up(), 0.0f);
    CHECK(ApproxEqual(axisAngleZero.w, 1.0f), "FromAxisAngle(Up, 0 radian) donne un quaternion identité");
}

void TestMatrix4x4()
{
    SECTION("Matrix4x4");

    Matrix4x4 identity = Matrix4x4::Identity();
    CHECK(ApproxEqual(identity.m.m[0][0], 1.0f) && ApproxEqual(identity.m.m[1][1], 1.0f) && ApproxEqual(identity.m.m[2][2], 1.0f) && ApproxEqual(identity.m.m[3][3], 1.0f),
          "La diagonale de l'identité vaut 1");
    CHECK(ApproxEqual(identity.m.m[0][1], 0.0f) && ApproxEqual(identity.m.m[1][0], 0.0f),
          "Les éléments hors diagonale de l'identité valent 0");

    Matrix4x4 result = Matrix4x4::Multiply(Matrix4x4::Identity(), Matrix4x4::Identity());
    CHECK(ApproxEqual(result.m.m[0][0], 1.0f) && ApproxEqual(result.m.m[3][3], 1.0f),
          "Identity * Identity == Identity");
}

void TestTransform()
{
    SECTION("Transform");

    Transform t;
    CHECK_VEC3_EQ(t.GetPosition(), Vector3::Zero(), "Position par défaut == (0,0,0)");
    CHECK_VEC3_EQ(t.GetScale(), Vector3::One(), "Scale par défaut == (1,1,1)");

    CHECK_VEC3_EQ(t.GetForward(), Vector3::Forward(), "Forward par défaut (sans rotation) == (0,0,1)");
    CHECK_VEC3_EQ(t.GetRight(), Vector3::Right(), "Right par défaut (sans rotation) == (1,0,0)");
    CHECK_VEC3_EQ(t.GetUp(), Vector3::Up(), "Up par défaut (sans rotation) == (0,1,0)");

    t.SetPosition(Vector3(1.0f, 2.0f, 3.0f));
    CHECK_VEC3_EQ(t.GetPosition(), Vector3(1.0f, 2.0f, 3.0f), "SetPosition() met bien à jour la position");

    Transform t2;
    t2.Translate(Vector3(1.0f, 0.0f, 0.0f));
    t2.Translate(Vector3(0.0f, 2.0f, 0.0f));
    CHECK_VEC3_EQ(t2.GetPosition(), Vector3(1.0f, 2.0f, 0.0f), "Translate() s'accumule correctement");

    // World Matrix : la translation doit se retrouver dans la dernière ligne
    // (convention DirectXMath : matrices row-major, vecteurs ligne)
    Transform t3;
    t3.SetPosition(Vector3(5.0f, 0.0f, 0.0f));
    Matrix4x4 world = t3.GetWorldMatrix();
    CHECK_FLOAT_EQ(world.m.m[3][0], 5.0f, "La translation en X apparaît dans la World Matrix (ligne 3)");
    CHECK_FLOAT_EQ(world.m.m[3][1], 0.0f, "Pas de translation parasite en Y");

    // Rotation : après une rotation, Forward/Right doivent rester orthogonaux et unitaires
    Transform t4;
    t4.SetRotation(Vector4::FromEuler(0.0f, XM_PIDIV2, 0.0f)); // 90° en yaw
    Vector3 forward4 = t4.GetForward();
    Vector3 right4 = t4.GetRight();
    CHECK_FLOAT_EQ(forward4.Length(), 1.0f, "Forward reste unitaire après rotation");
    CHECK_FLOAT_EQ(Vector3::Dot(forward4, right4), 0.0f, "Forward et Right restent orthogonaux après rotation");
    CHECK(!ApproxEqual(forward4, Vector3::Forward()), "Forward a bien changé après une rotation de 90°");

    // Une rotation complète (360°) doit ramener Forward à sa valeur de départ
    Transform t5;
    t5.SetRotation(Vector4::FromEuler(0.0f, XM_2PI, 0.0f));
    CHECK_VEC3_EQ(t5.GetForward(), Vector3::Forward(), "Une rotation de 360° ramène Forward à sa position initiale");
}

void TestCamera()
{
    SECTION("Camera");

    Camera cam;
    CHECK_VEC3_EQ(cam.GetPosition(), Vector3(0.0f, 0.0f, -5.0f), "Position initiale de la caméra == (0,0,-5)");

    Vector3 posBefore = cam.GetPosition();
    cam.MoveForward(2.0f);
    Vector3 posAfter = cam.GetPosition();
    CHECK_FLOAT_EQ((posAfter - posBefore).Length(), 2.0f, "MoveForward(2) déplace la caméra de 2 unités");

    cam.SetProjection(XM_PIDIV4, 16.0f / 9.0f, 0.1f, 100.0f);
    const Matrix4x4 &proj = cam.GetProjectionMatrix();
    // Signature d'une matrice de projection perspective LH valide (DirectXMath) :
    // m[2][3] == 1 et m[3][3] == 0 (contrairement à l'identité)
    CHECK_FLOAT_EQ(proj.m.m[2][3], 1.0f, "La Projection Matrix a la forme d'une perspective valide (m[2][3] == 1)");
    CHECK_FLOAT_EQ(proj.m.m[3][3], 0.0f, "La Projection Matrix a la forme d'une perspective valide (m[3][3] == 0)");

    Matrix4x4 viewBefore = cam.GetViewMatrix();
    cam.Rotate(0.5f, 0.0f);
    cam.Update();
    Matrix4x4 viewAfter = cam.GetViewMatrix();
    bool viewChanged = !ApproxEqual(viewBefore.m.m[0][0], viewAfter.m.m[0][0]) || !ApproxEqual(viewBefore.m.m[2][0], viewAfter.m.m[2][0]);
    CHECK(viewChanged, "Rotate() + Update() modifient bien la View Matrix");
}

void TestQuaternion()
{
    SECTION("Quaternion");

    Quaternion id = Quaternion::Identity();
    CHECK(ApproxEqual(id.w, 1.0f) && ApproxEqual(id.x, 0.0f) && ApproxEqual(id.y, 0.0f) && ApproxEqual(id.z, 0.0f),
          "Quaternion::Identity() == (w=1, x=0, y=0, z=0)");

    // Normalisation : un quaternion "gonflé" doit revenir à une norme de 1
    Quaternion notUnit(2.0f, 0.0f, 0.0f, 0.0f);
    CHECK_FLOAT_EQ(notUnit.Normalized().Length(), 1.0f, "Normalized() donne toujours un quaternion de longueur 1");

    // Produit hamiltonien : q * Identity == q (l'identité est neutre)
    Quaternion q = Quaternion::FromAxisAngle(Vector3::Up(), XM_PIDIV2);
    Quaternion qTimesId = q * Quaternion::Identity();
    CHECK(ApproxEqual(qTimesId.w, q.w) && ApproxEqual(qTimesId.x, q.x) && ApproxEqual(qTimesId.y, q.y) && ApproxEqual(qTimesId.z, q.z),
          "q * Identity == q (élément neutre du produit hamiltonien)");

    // Non-commutativité : deux rotations à 90° sur des axes différents ne
    // donnent PAS le même résultat selon l'ordre.
    Quaternion rotY = Quaternion::FromAxisAngle(Vector3::Up(), XM_PIDIV2);
    Quaternion rotX = Quaternion::FromAxisAngle(Vector3::Right(), XM_PIDIV2);
    Quaternion ab = rotY * rotX;
    Quaternion ba = rotX * rotY;
    bool sameResult = ApproxEqual(ab.w, ba.w) && ApproxEqual(ab.x, ba.x) && ApproxEqual(ab.y, ba.y) && ApproxEqual(ab.z, ba.z);
    CHECK(!sameResult, "Le produit hamiltonien n'est PAS commutatif (a*b != b*a)");

    // Conjugué d'un quaternion unitaire == son inverse
    Quaternion conj = q.Conjugate();
    Quaternion inv = q.Inverse();
    CHECK(ApproxEqual(conj.w, inv.w) && ApproxEqual(conj.x, inv.x) && ApproxEqual(conj.y, inv.y) && ApproxEqual(conj.z, inv.z),
          "Pour un quaternion unitaire, Conjugate() == Inverse()");

    // q * q.Inverse() == Identity
    Quaternion shouldBeIdentity = q * q.Inverse();
    CHECK(ApproxEqual(shouldBeIdentity.w, 1.0f), "q * q.Inverse() == Identity (w == 1)");

    // Rotation d'un vecteur : 90° autour de Up doit envoyer Forward vers Right
    // (ou -Right selon le sens de rotation -- on vérifie juste la cohérence interne)
    Vector3 rotated = rotY.RotateVector(Vector3::Forward());
    CHECK_FLOAT_EQ(rotated.Length(), 1.0f, "RotateVector() préserve la longueur d'un vecteur unitaire");
    CHECK_FLOAT_EQ(Vector3::Dot(rotated, Vector3::Up()), 0.0f,
                   "Une rotation autour de Up ne fait pas sortir le vecteur du plan horizontal");

    // Cohérence interne : RotateVector() et ToMatrix4x4() doivent produire
    // exactement le même résultat, puisqu'ils représentent la même rotation.
    Matrix4x4 rotMatrix = rotY.ToMatrix4x4();
    Vector3 rotatedByMatrix = rotMatrix.TransformVector(Vector3::Forward());
    CHECK_VEC3_EQ(rotated, rotatedByMatrix,
                  "RotateVector() et ToMatrix4x4()+TransformVector() donnent le même résultat");

    // Aller-retour Quaternion -> Matrice -> Quaternion (méthode de Shepperd)
    Quaternion roundTrip = Quaternion::FromMatrix4x4(q.ToMatrix4x4());
    // q et -q représentent la même rotation, donc on accepte les deux signes.
    bool sameQuat = (ApproxEqual(roundTrip.w, q.w) && ApproxEqual(roundTrip.x, q.x) && ApproxEqual(roundTrip.y, q.y) && ApproxEqual(roundTrip.z, q.z)) || (ApproxEqual(roundTrip.w, -q.w) && ApproxEqual(roundTrip.x, -q.x) && ApproxEqual(roundTrip.y, -q.y) && ApproxEqual(roundTrip.z, -q.z));
    CHECK(sameQuat, "FromMatrix4x4(ToMatrix4x4(q)) == q (aller-retour, à un signe près)");

    // Pas de blocage de Gimbal : une rotation composée à 90° de pitch (cas
    // qui bloquerait des angles d'Euler classiques) reste bien définie et
    // unitaire.
    Quaternion gimbalCase = Quaternion::FromEulerAngles(XM_PIDIV2, XM_PIDIV4, XM_PIDIV4);
    CHECK_FLOAT_EQ(gimbalCase.Normalized().Length(), 1.0f,
                   "Une composition pitch=90° + yaw + roll reste un quaternion valide (pas de division par 0)");
}

int main()
{
    printf("=== Tests Vector3 / Vector4 / Matrix4x4 / Transform / Camera / Quaternion ===\n");

    TestVector3();
    TestVector4();
    TestMatrix4x4();
    TestTransform();
    TestCamera();
    TestQuaternion();

    printf("\n=== Résumé : %d réussi(s), %d échoué(s) ===\n", g_testsPassed, g_testsFailed);

    return g_testsFailed == 0 ? 0 : 1;
}