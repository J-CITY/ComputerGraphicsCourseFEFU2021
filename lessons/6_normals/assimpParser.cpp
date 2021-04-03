
bool AssimpParser::LoadModel(const std::string& fileName, std::vector<Mesh*>& meshes, std::vector<std::string>& materials, EModelParserFlags flags) {
	
	Assimp::Importer import;
	const auto scene = import.ReadFile(fileName, static_cast<unsigned int>(flags));
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return false;
	}

	ProcessMaterials(scene, materials);

	aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, meshes);
	return true;
}

void AssimpParser::ProcessMaterials(const aiScene* scene, std::vector<std::string>& materials) {
	for (uint32_t i = 0; i < scene->mNumMaterials; ++i) {
		aiMaterial* material = scene->mMaterials[i];
		if (material) {
			aiString name;
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			materials.push_back(name.C_Str());
		}
	}
}

void AssimpParser::ProcessNode(void* transform, aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes) {
	aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(transform) * node->mTransformation;
	
	for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(&nodeTransformation, mesh, scene, vertices, indices);
		meshes.push_back(new Mesh(vertices, indices, mesh->mMaterialIndex));
	}

	// Then do the same for each of its children
	for (uint32_t i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(&nodeTransformation, node->mChildren[i], scene, meshes);
	}
}

void AssimpParser::ProcessMesh(void* transform, aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) {
	aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(transform);

	for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
		aiVector3D position = meshTransformation * mesh->mVertices[i];
		aiVector3D normal = meshTransformation * (mesh->mNormals ? mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
		aiVector3D texCoords = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D tangent = mesh->mTangents ? meshTransformation * mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D bitangent = mesh->mBitangents ? meshTransformation * mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

		outVertices.push_back
		(
			{
				Mathgl::Vector3{
					position.x,
					position.y,
					position.z
				},
				Mathgl::Vector2{
					texCoords.x,
					texCoords.y
				},
				Mathgl::Vector3{
					normal.x,
					normal.y,
					normal.z
				},
				Mathgl::Vector3{
					tangent.x,
					tangent.y,
					tangent.z
				},
				Mathgl::Vector3{
					bitangent.x,
					bitangent.y,
					bitangent.z
				}
			}
		);
	}

	for (uint32_t faceID = 0; faceID < mesh->mNumFaces; ++faceID) {
		auto& face = mesh->mFaces[faceID];
		for (size_t indexID = 0; indexID < 3; ++indexID)
			outIndices.push_back(face.mIndices[indexID]);
	}
}

