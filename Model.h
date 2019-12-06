#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "loadBox.h"


class Model
{
	public:

		/*  Functions   */
		Model(char *path);
		void Draw(GLuint shader);

	private:

		/*  Model Data  */
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;

		/*  Functions   */
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
				std::string typeName);
};

#endif