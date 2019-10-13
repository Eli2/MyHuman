#include "MhRender.h"

#include "GlInclude.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gui/CGUtilities.h"
#include "gui/Window.h"
#include "render/RenderUtils.h"
#include "Logger.h"
#include "render/Shader.h"
#include "Global.h"


std::optional<mh::Shader> g_bodyShader;
std::optional<mh::Shader> g_backgroundShader;


void loadTextures()
{
	for(auto & [name, value] : g_mesh.facegroup().m_groups) {
		
		std::string fileName = "data/pixmaps/ui/" + name + "_color.png";
		value.texture        = LoadTextureFromFile(fileName.c_str());
		if(!value.texture) {
			log_error("couldn't load base skin_color Texture Data {}_color.png", name);
		}
		
		
		{
			std::string dir = "data/data/rib_data/textures_data/";
			value.specular  = LoadTextureFromFile(dir + name + "_specular.png");
			value.bump      = LoadTextureFromFile(dir + name + "_bump.png");
		}
	}
}

void LoadBodyShader(int version)
{
	log_info("Loading Shader set {}", version);
	
	std::optional<mh::Shader> shader;
	if(version == 1) {
		shader = LoadShader("data/shader/body.vert", "data/shader/body.frag");
	} else {
		shader = LoadShader("data/shader/body_2.vert", "data/shader/body_2.frag");
	}
	
	if(shader) {
		glDeleteProgram(g_bodyShader->handle);
		g_bodyShader = shader.value();
	}
}

RenderBackground g_renderBackground;

void RenderBackground::init() {
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
		glGenBuffers(1, &m_vertexBufferObject);
		
		glBindVertexArray(m_vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		
		static constexpr GLfloat bufferData[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			-1.0f,  1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
	}
	loadShader();
}

void RenderBackground::loadShader()
{
	log_info("Loading Background Shader");
	
	auto shader = LoadShader("data/shader/background.vert", "data/shader/background.frag");
	
	if(shader) {
		glDeleteProgram(g_backgroundShader->handle);
		g_backgroundShader = shader.value();
	}
}

void RenderBackground::render()
{
	using glm::vec2;
	
	auto inSize = mhgui::g_mainWindow->getSize();
	vec2 size = vec2(inSize.x, inSize.y);
	
	glUseProgram(g_backgroundShader->handle);
	GLint myLoc = glGetUniformLocation(g_backgroundShader->handle, "u_viewportResolution");
	if(myLoc != -1) {
		glProgramUniform2f(g_backgroundShader->handle, myLoc, size.x, size.y);
	}
	
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glUseProgram(0);
}

void renderMesh()
{
	using Animorph::MaterialVector;
	using Animorph::TextureVector;
	using Animorph::FaceVector;
	using Animorph::Face;
	using Animorph::TextureFace;
	using Animorph::Material;
	using Animorph::Vertex;
	
	
	const MaterialVector & materialvector(g_mesh.materials());
	const TextureVector &  texturevector(g_mesh.textureVector());
	
	const FaceVector & facevector(g_mesh.faces());
	
	const VertexVector & vertexvector(g_mesh.getVertexVectorRef());
	
	int istri = -1; // to remember which type was the latest drawn geometry and
	    // avoid too many glBegin
	
	cgutils::enableBlend();
	
	glUseProgram(g_bodyShader->handle);
	
	for(auto & [goupName, groupValue] : g_mesh.facegroup().m_groups) {
		
		if(groupValue.visible == false)
			continue;
		
		if(g_global.m_enableTexture) {
			if(groupValue.texture) {
				glActiveTexture(GL_TEXTURE0);
				::glBindTexture(GL_TEXTURE_2D, groupValue.texture.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture0"),
				            0);
			}
			if(groupValue.specular) {
				glActiveTexture(GL_TEXTURE1);
				::glBindTexture(GL_TEXTURE_2D, groupValue.specular.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture1"),
				            1);
			}
			if(groupValue.bump) {
				glActiveTexture(GL_TEXTURE2);
				::glBindTexture(GL_TEXTURE_2D, groupValue.bump.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture2"),
				            2);
			}
		}
		
		for(const auto & faceIndex : groupValue.facesIndexes) {
			const Face &        face         = facevector[faceIndex];
			const TextureFace & texture_face = texturevector[faceIndex];
			
			int facesize = (int)face.getSize();
			
			if(istri != facesize) {
				if(istri != -1) {
					::glEnd();
				}
				
				::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
				istri = facesize;
			}
			
			int material_index = face.getMaterialIndex();
			if(material_index != -1) {
				const Material & material(materialvector[material_index]);
				const Color &    color(material.color);
				
				// Set the color for these vertices
				::glColor4fv(color.getAsOpenGLVector());
			}
			
			for(size_t j = 0; j != face.getSize(); ++j) {
				const int & vertIndex = face.vertices[j];
				
				const Vertex & vertex = vertexvector.m_verts[vertIndex];
				const glm::vec3 & normal = vertexvector.m_normals[vertIndex];
				const glm::vec2 & uv = texture_face[j];
				
				::glNormal3fv(glm::value_ptr(normal));
				::glTexCoord2f(uv.x, uv.y);
				::glVertex3fv(glm::value_ptr(vertex.pos));
			}
		}
		
		::glEnd();
		istri = -1;
	}
	
	glUseProgram(0);
	
	//	glColor3ub( 255, 255, 255 );
	//	for(const auto & strip: mesh->edgestrip) {
	//		glBegin(GL_LINE_STRIP);
	//		for(const auto & index: strip) {
	//			const Vertex &vertex = vertexvector[index];
	//			::glVertex3fv(glm::value_ptr(vertex.co));
	//		}
	//		glEnd();
	//	}
	
	glDisable(GL_BLEND);
}
