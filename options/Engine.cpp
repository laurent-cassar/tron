#include "Engine.h"
#include "MeshComponent.h"

void Engine::RegisterGameObject(std::unique_ptr<GameObject> gameObject) {
    if (gameObject) {
        m_GameObjects.push_back(std::move(gameObject));
    }
}

void Engine::SetActiveCamera(CameraComponent* camera) {
    m_ActiveCamera = camera;
}

void Engine::Update(float deltaTime) {
    // 1. Appliquer le multiplicateur de temps pour le ralenti (Contrainte du sujet)
    // float customDeltaTime = deltaTime * m_TimeScale;

    // 2. Mettre à jour tous les scripts (PlayerController, Ennemis, etc.)
    for (auto& gameObject : m_GameObjects) {
        // On récupère tous les composants de type ScriptBehaviour de l'objet
        auto scripts = gameObject->GetComponents<ScriptBehaviour>();
        for (auto* script : scripts) {
            script->Update(deltaTime);
        }
    }

    // 3. C'est ici que tu pourras appeler tes futurs systèmes (ex: Gestion des collisions)
}

void Engine::Render() {
    // Sécurité : Impossible de faire un rendu 3D sans caméra active
    if (!m_ActiveCamera) return;

    // Nettoyage des buffers Direct3D (ex: ClearRenderTargetView...)

    // Passer les matrices de la caméra active aux Shaders Direct3D

    // Parcourir tous les objets pour dessiner ceux qui ont un MeshComponent
    for (auto& gameObject : m_GameObjects) {
        auto* meshComp = gameObject->GetComponent<MeshComponent>();
        if (meshComp) {
            Mesh* meshToRender = meshComp->GetMesh();
            // Appeler le code de rendu Direct3D de ton collègue en lui passant le meshToRender
            // ex: m_DirectXRenderer->DrawMesh(meshToRender, gameObject->GetComponent<TransformComponent>());
        }
    }

    // Présentation de la frame (Present)
}