#include "Texture.h"

GLuint Texture::bikeTexture, Texture::planeTexture;
ModelVertices Texture::bikeModel;

void Texture::loadResources()
{
	bikeTexture = loadTGATexture("textures/bike_text.tga");
	planeTexture = loadTGATexture("textures/square_plane.tga");
	
	std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> colors;	
	
	if(!Texture::loadOBJ("objs/bike.obj",vertices, uvs,normals))
	{
		printf("Error loading bike.obj\n");
	}

	indexVBO(vertices, uvs, normals, bikeModel.indices, bikeModel.vertices, bikeModel.uvs, bikeModel.normals); 	
}

GLuint Texture::loadTGATexture(const std::string& imagepath, GLenum wrap_s, GLenum wrap_t, GLenum mag_filter, GLenum min_filter, bool anisotropy)
{
    int width, height, depth;
    char * datas = read_tga(imagepath.data(), width, height, depth);

    GLuint textureID=0;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, datas);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

	glGenerateMipmap(GL_TEXTURE_2D);
	if(anisotropy)
	{
		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
	}

    return textureID;
}

char *Texture::read_tga(const char *filename, int& width, int& height, int& depth)/*{{{*/
{
    struct tga_header {
       char  id_length;
       char  color_map_type;
       char  data_type_code;
       unsigned char  color_map_origin[2];
       unsigned char  color_map_length[2];
       char  color_map_depth;
       unsigned char  x_origin[2];
       unsigned char  y_origin[2];
       unsigned char  width[2];
       unsigned char  height[2];
       char  bits_per_pixel;
       char  image_descriptor;
    } header;
    int i, color_map_size;
    FILE *f;
    size_t read;
    char *pixels;

    f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24 and header.bits_per_pixel != 32) {
        fprintf(stderr, "%s is not a 24-bit or 32-bit uncompressed RGB tga file but %i\n", filename, header.bits_per_pixel);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    width = le_short(header.width); height = le_short(header.height);
    depth = (header.bits_per_pixel/8);
    unsigned int pixels_size = width * height * depth;
    pixels = new char[pixels_size];

    read = fread(pixels, 1, pixels_size, f);

    if (read != pixels_size) {
        fprintf(stderr, "%s has incomplete image\n", filename);
        fclose(f);
        delete [] pixels;
        return NULL;
    }

    return pixels;
}

bool Texture::loadOBJ(const char * path,std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs,std::vector<glm::vec3> & out_normals)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}

	return true;
}

short Texture::le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}
