Here's the improved `README.md` file, incorporating the new content while preserving the existing structure and information:

# EthernalEngine

EthernalEngine is a lightweight, modular OpenGL-based game engine and editor written in modern C++. This README documents the project's features, architecture, build instructions, workflows, and contribution guidelines so contributors and users can quickly get started.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Key Features](#key-features)
- [Architecture and Modules](#architecture-and-modules)
- [Rendering Pipeline](#rendering-pipeline)
- [Scene Graph & Components](#scene-graph--components)
- [Assets & Shaders](#assets--shaders)
- [How to Build (Windows / Visual Studio 2022 + Ninja)](#how-to-build-windows--visual-studio-2022--ninja)
- [Running the Editor / Sample](#running-the-editor--sample)
- [Debugging & Common Issues](#debugging--common-issues)
- [Coding Standards & Project Conventions](#coding-standards--project-conventions)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## Project Overview

EthernalEngine provides a minimal but practical foundation for real-time rendering and simple game/editor workflows. The engine focuses on clarity and extensibility: a small core, modular rendering, a scene graph, a component system (lights, camera), and an integrated editor/debug drawing helpers.

This repository contains the engine sources, asset shaders, third-party dependencies (packed under `Dependencies/`), and example assets.

## Key Features

- Cross-platform C++ codebase (primary dev/test on Windows Visual Studio)
- OpenGL renderer (GLAD + GLFW) with shader-based lighting
- Material system that centralizes shader, texture, and per-object uniforms
- Scene graph with hierarchical GameObjects and transforms
- Component system with built-in lights: Directional, Point, Spot
- Model importer using Assimp (supports embedded and file-based textures)
- Simple editor helpers and debug drawing utilities
- Shader examples demonstrating directional, point, and spot lighting
- Texture loading via stb_image
- ImGui integrated for editor UI (assets/third-party included)

## Architecture and Modules

The code is organized into clear subsystems. Primary directories and responsibilities:

- `Include/` — Public engine headers (Rendering, Scene, Components, Core, etc.)
- `Source/` — Engine implementations corresponding to headers
- `Assets/` — Shaders, textures, and sample assets
- `Dependencies/` — Bundled third-party libs (Assimp, GLAD, GLFW, stb_image, ImGui, etc.)

Core concepts:

- **EngineCamera**: Central camera used for rendering and picking.
- **GameObject**: Hierarchical scene node holding Transform, optional Mesh or Model, Material, and Components.
- **Material**: Owns `Shader*`, base texture, and material parameters (color, shininess). Responsible for updating shader uniforms and binding textures prior to draw.
- **Mesh / Model**: Geometry primitives and imported models (Assimp). `Model` creates child `GameObject`s for each mesh so they participate in scene graph traversal.
- **Renderer**: Traverses root GameObjects in `Scene`, updates materials, and issues draw calls. Debug gizmo rendering and light visualization helpers included.
- **Components**: Light classes for Directional, Point, Spot providing properties (color, intensity, radius, specular) and debug drawing.

Design notes:
- Per-object uniforms (model matrix, color multiplier, view position) are set by `Material::Update` before each draw.
- Textures and shader bindings moved to `Material` so shader usage is centralized.
- Models are represented both as owner `Model` and as `GameObject` children with their own `Material` instances.

## Rendering Pipeline

- Shaders are GLSL 330 core.
- Vertex shader computes world-space position, normal, and passes TexCoords and a per-object `vertexColor` derived from `colorMultiplier`.
- Fragment shader implements lighting using:
  - Scene ambient term (scene color + intensity)
  - Directional light (ambient/diffuse/specular)
  - Indexed point lights (attenuation, specular)
  - Indexed spot lights (cutoff, attenuation, specular)
- Mesh vertex attributes: position (location 0), normal (1), uv (2). Ensure meshes provide normals and UVs.
- Material responsibilities:
  - Activate shader: `shader->Use()`
  - Set global and per-object uniforms: `view`, `projection`, `model`, `viewPos`, ambient, lights
  - Bind base texture to `GL_TEXTURE0` and set `image` sampler uniform to 0

## Scene Graph & Components

- `Scene` maintains root GameObjects and lists of lights. It exposes:
  - `GetGameObjects()`, `GetDirectionalLight()`, `GetPointLights()`, `GetSpotLights()`
  - `CreateGameObjectWithCustomModel(name, path)` uses `Model` which uses Assimp to import meshes and create child `GameObject`s with `Material`s.
- `Renderer` traverses scene roots and recursively visits children. Materials are updated for each object prior to draw.
- `GameObject` stores: `Transform`, `mesh` or `model`, `material`, `childObjects`, and `components`.

## Assets & Shaders

Key shader files:
- `Assets/Shaders/Shader.vert` — Vertex shader: outputs `TexCoords`, `vertexColor`, `Normal`, `FragPos`.
- `Assets/Shaders/Shader.frag` — Fragment shader: lighting calculations for directional/point/spot lights and texture sampling.
- `Assets/Shaders/DebugShader.frag` — Simple debug fragment shader writing vertex color.

Textures:
- `Textures/White.png` — Default white texture used by `CreateCubeGameObject`.

Model import notes:
- The `Model` loader uses Assimp with post-process flags: `aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices`.
- For each `aiMesh`, the loader constructs a `Mesh` (vertices, normals, UVs, indices), creates a `Material`, sets a base texture (embedded or file), and creates a child `GameObject` to hold that mesh and material.

## How to Build

Prerequisites:
- Windows with Visual Studio 2022 (MSVC toolset)
- CMake >= 3.20 (project uses CMake 3.31.6-msvc6 features)
- Ninja generator (recommended) or Visual Studio generator

Build steps (Ninja):

1. Open a Developer Command Prompt for Visual Studio or a terminal with MSVC on PATH.
2. Create a build directory at project root:
   mkdir build
   cd build
3. Configure with CMake (Ninja generator):
   cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
4. Build:
   ninja
5. Run the resulting executable (path found in build output). For Visual Studio generator, open the generated solution and build.

Notes:
- If third-party dependencies are not bundled, ensure you install or configure Assimp, GLFW, GLAD, stb_image and point CMake variables accordingly.
- The codebase expects GLSL shader files at `Shaders/Shader.vert` and `Shaders/Shader.frag` relative to working directory; ensure the working directory contains the `Assets/` folder or copy shaders next to the executable.

## Running the Editor / Sample

- Start the executable from the build folder (or run from IDE). The engine should initialize OpenGL context and load default resources.
- The sample scene typically creates a `Cube` using `CreateCubeGameObject` with the default material and texture. Use the scene UI to add lights, import models, and inspect object transforms.

## Debugging & Common Issues

- **Black objects / no lighting**:
- Ensure `Material::Update` is called before `GameObject::Draw` for each object. Renderer should traverse and call `mat->Update(scene, *obj)` per object.
- Confirm `Shader` is active (`shader->Use()`) when setting uniforms.
- Ensure `colorMultiplier` uniform type matches (vertex shader expects `vec3 colorMultiplier`, so set via `SetFloat3`).
- Confirm textures are bound to `GL_TEXTURE0` and that shader sampler `image` is set to 0.
- Verify meshes have valid normals and UVs; Assimp flag `aiProcess_GenSmoothNormals` can generate normals.

- **Model not visible after import**:
- Confirm imported model root `GameObject` is added to `Scene` root via `AddGameObject` or included in scene graph.
- Check that `Model::ProcessMesh` creates child `GameObject`s and assigns `Material` to them.
- Confirm `Renderer` recursively traverses and updates/draws child `GameObject`s.

- **Uniform location -1**: If `glGetUniformLocation` returns -1, uniform is either optimized out (unused in shader) or name mismatch. Ensure uniform names match exactly.