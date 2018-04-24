#include "Entity.h"

Entity::Entity(vector3 pos, GLfloat angle){
    Entity::pos = pos;
    Entity::angle = angle;
}

Entity::Entity(vector3 pos, vector3 dir, vector3 up){
    Entity::pos = pos;//+vector3(0.0,0.02,0.0);
    Entity::dir = dir;
    Entity::up = up;
    dirMat[12]=0; dirMat[13]=0; dirMat[14]=0;
    dirMat[3]=0; dirMat[7]=0; dirMat[11]=0; dirMat[15]=1;
    updateDirMat();
}

void Entity::updateDirMat(){
    dir.normalize();
    vector3 right = cross(up, dir);
    right.normalize();
    dirMat[0]=right[0];   dirMat[4]=up[0]; dirMat[8]=dir[0];   //dirMat[12]=0;
    dirMat[1]=right[1];   dirMat[5]=up[1]; dirMat[9]=dir[1];   //dirMat[13]=0;
    dirMat[2]=right[2];   dirMat[6]=up[2]; dirMat[10]=dir[2];  //dirMat[14]=0;
    //dirMat[3]=0;        dirMat[7]=0;     dirMat[11]=0;         dirMat[15]=1;
}

vector3 Entity::getPos(){
    return pos;
}
void Entity::setPos(vector3 pos){
    Entity::pos = pos;
}

void Entity::setDir(vector3 dir){
    Entity::dir = dir;
}

vector3 Entity::getDir(){
    return dir;
}

void Entity::setUp(vector3 up){
    Entity::up = up;
}

vector3 Entity::getUp(){
    return up;
}

GLfloat* Entity::getBBox(){
    return bbox;
}
void Entity::setBBox(GLfloat* bbox){
    Entity::bbox = bbox;
}

GLuint Entity::LoadTextureRAW(string filename) {
    GLuint texture;
    //formato da imagem
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//ponteiro p/ imagem, 01 vez carregada
	FIBITMAP *dib(0);
	//ponteiro pros dados da imagem
	BYTE* bits(0);
	//altura e largura da imagem
	unsigned int width(0), height(0);


    fif = FreeImage_GetFileType(filename.c_str(), 0);
	//se o formato da imagem for desconhecido, tenta descobrir qual é
	if(fif == FIF_UNKNOWN){
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
		cout << "Pegando o formato da imagem" << endl;
	}
	//se mesmo assim não conseguir, retorna falha
	if(fif == FIF_UNKNOWN) cout << "Falha ao pegar o formato da imagem" << endl;

	//checa as capacidades de leitura do plugin e carrega o arquivo
	if(FreeImage_FIFSupportsReading(fif)){
		//dib = FreeImage_ConvertTo32Bits(dib);
		dib = FreeImage_Load(fif, filename.c_str());
	}
	//se o load falhar, retorna falha
	if(!dib) cout << "Failed to load image" << endl;


    bits = FreeImage_GetBits(dib);
    width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

    // aloca um nome p/ textura
    glGenTextures(1, &texture);


    glBindTexture(GL_TEXTURE_2D, texture); //Fala pro OpenGL qual textura tem que editar
        //Mapeia a imagem pra textura
    glTexImage2D(GL_TEXTURE_2D,                //Sempre GL_TEXTURE_2D
                 0,                            //0 por agora
                 GL_RGB,                       //Formato que o OpenGL usa pras imagens
                 width, height,  //Largura e altura
                 0,                            //A borda da imagem
                 GL_BGR, //GL_RGB, porque os píxels são guardados no formato RGB
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, porque os pixels são armazenados
                                   //como unsigned numbers
                 bits);               //Os dados atuais dos píxels

    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    FreeImage_Unload(dib);
    return texture;
}


GLuint Entity::LoadTextureRGBA(string filename) {
    GLuint texture;
    //formato da imagem
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filename.c_str(), 0);
	FIBITMAP *dib = FreeImage_Load(fifmt, filename.c_str(),0);
	if( dib != NULL ){
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
               FreeImage_GetWidth(dib), FreeImage_GetHeight(dib),
                0, GL_BGRA, GL_UNSIGNED_BYTE, pixels );

		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		FreeImage_Unload(dib);
	}
	return texture;
}
