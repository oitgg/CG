#include "Character.h"

Character::Character(vector3 pos, GLfloat mass, EntityList *entList)
         :Item(pos,vector3(0.0,0.0,-1.0), mass, vector3(0.0,0.0,0.0), entList)
{
    releaseForce = vector3(0.0,5.0,0.0);
    shotForce = 20.0;
    at=vector3(0.0,0.0,0.0);
    dir1=0.0;

    moveForward=false; moveBackward=false; moveRight=false; moveLeft=false;
    t0 = glutGet(GLUT_ELAPSED_TIME);
    jump = true; run = false;
    takenEntity[0]= new Weapon(entList);
    takenEntity[1]=NULL;
}

void Character::init(Portal* blue, Portal* orange){
    takenEntity[2]= new Interface(blue, orange);
    Character::blue = blue;
    Character::orange = orange;
    //cout << pos << endl;
 	std::vector<vector3> walls;
	std::vector<Floor*> floors;
	collision(&walls, &floors);
    //acha o andar atual
    std::vector<Floor*>::iterator its = floors.begin();
    Floor* below_floor=*its;
    GLfloat below_height = below_floor->get_height(pos);
    for(its++; its!=floors.end(); its++){
        Floor* fl = *its;
        GLfloat floor_height = fl->get_height(pos);
        if(floor_height>=pos[1] && floor_height<=below_height){
            below_floor=fl;
            below_height=floor_height;
        }
    }
    actual_floor=below_floor;
}

void Character::mouse(int button, int state, int x, int y){
    if(takenEntity[1]!=NULL){
            throwEntity();
    }else{
        if (state==GLUT_DOWN){
            vector3 portal_pos;
            vector3 portal_dir=vector3(0.0,0.0,0.0);
            vector3 portal_up;
            (dynamic_cast<Weapon*>(takenEntity[0]))->
            shot_portal(vector3(pos[0],pos[1]+1.8,pos[2]), vector3(dir[0],dir1,dir[2]),&portal_pos,&portal_dir,&portal_up);

            //cout << portal_pos << " " << portal_dir << " " << portal_up << endl;
            if(button==GLUT_LEFT_BUTTON && portal_dir!=vector3(0.0,0.0,0.0)){
                  blue->set_position(portal_pos, portal_dir, portal_up);
            }
            if(button==GLUT_RIGHT_BUTTON && portal_dir!=vector3(0.0,0.0,0.0)){
                  orange->set_position(portal_pos, portal_dir, portal_up);
            }
        }
    }
}

void Character::keyboard(unsigned char key)
{
	switch (key) {
	    /* Exit (ESC)  */
        case 27: exit (0); break;
        /* Frente (W) */
        case 'w': moveForward = true; FB=1.0; break;
        /* Trás (S) */
        case 's': moveBackward = true; FB=-1.0; break;
        /* Direita (D) */
        case 'd': moveRight = true; RL=-1.0; break;
        /* Esquerda (A) */
        case 'a': moveLeft = true; RL=1.0; break;
        /* Pula (Espaço) */
        case 32:
            if(!jump){
                jump = true;
                t0 = glutGet(GLUT_ELAPSED_TIME);
                v0 = vector3(0.0,300.0,0.0)*(1/mass)*0.5;
            }
            break;
        default:
            break;
	}
}

void Character::keyboardUp(unsigned char key)
{

	switch ( key ) {
        /* Frente (W) */
        case 'w': moveForward = false; break;
        /* Trás (S) */
        case 's': moveBackward = false; break;
        /* Direita (D) */
        case 'd': moveRight = false; break;
       /* Esquerda (A) */
        case 'a': moveLeft = false; break;
        /* Pegar (E) */
        case 'e':
            if(takenEntity[1]==NULL) takeEntity();
            else releaseEntity();
            break;
        default:
            break;
	}
}

void Character::specialKeyboard(int key){
}

void Character::movement(){
    float interval = pe.idle()/100;
    vector3 tmp = dir;
    vector3 tmp2 = dir;
    vector3 tmp3 = side;

	std::vector<vector3> walls;
	std::vector<Floor*> floors;
	Portal* teleport_portal = collision(&walls, &floors);

    //teleporting
	if(teleport_portal!=NULL){
        vector3 other_dir = teleport_portal->other_portal->getDir();
        GLfloat rot_angle = asin(dir[0]*other_dir[2]-dir[2]*other_dir[0]);

        if(teleport_portal->getDir()[1]==0.0){
            pos = teleport_portal->getPos()+teleport_portal->getDir()*1.5;
            pos[1]-=0.9;
            dir[0] = teleport_portal->getDir()[0];
            dir[2] = teleport_portal->getDir()[2];
        }else
            pos = teleport_portal->getPos()+teleport_portal->getDir()*2.5;

        dir = rotate_vector(dir, vector3(0.0, 1.0, 0.0), -rot_angle);
        dir.normalize();
        if(moveBackward) dir = -dir;
        side = cross(-dir, up);
        side.normalize();

        at = pos + dir;

        jump=true;
        v0 = vector3(0.0,0.0,0.0);
        t0 = glutGet(GLUT_ELAPSED_TIME);
        return;
	}


    //colisão com a parede
    std::vector<vector3>::iterator it;
    for(it=walls.begin(); it!=walls.end(); it++){
        vector3 collider = *it;

        //perpendicular a colisão
        collider[0] = collider[0] + collider[2];
        collider[2] = collider[0] - collider[2];
        collider[0] = collider[0] - collider[2];
        collider[2] = -collider[2];
        collider[1] = 0;

        GLfloat t2l = tmp2.length();
        GLfloat t3l = tmp3.length();
        GLfloat cosangle1 = (tmp2[0]*collider[0]+tmp2[2]*collider[2])/(t2l);
        GLfloat sinangle1 = (tmp2[0]*collider[2]-tmp2[2]*collider[0])/(t2l);
        GLfloat cosangle2 = (tmp3[0]*collider[0]+tmp3[2]*collider[2])/(t3l);
        GLfloat sinangle2 = (tmp3[0]*collider[2]-tmp3[2]*collider[0])/(t3l);

        if((sinangle2<0.0 && moveRight) || (sinangle2>=0.0 && moveLeft))
            tmp3 = collider*cosangle2;
        if((sinangle1>0.0 && moveForward) || (sinangle1<=0.0 && moveBackward))
            tmp2 = collider*cosangle1;
    }
    tmp3[1]=0.0;
    tmp[1]=dir1;

    //acha o andar inferior
    std::vector<Floor*>::iterator its;
    Floor* below_floor;
    GLfloat below_height=-100.0;
    for(its=floors.begin(); its!=floors.end(); its++){
        Floor* fl = *its;
        GLfloat floor_height = fl->get_height(pos);
        //cout << pos[1] << " " << floor_height << " " << below_height << endl;
        if(floor_height-1.0<=pos[1] && floor_height>=below_height && fl->getDir()[1]>0.0){
            below_floor=fl;
            below_height=floor_height;
        }
    }
    //cout << "........." << endl;

    //reação do pulo e da aterrissagem
	if(jump){
        pos = pe.getProjectileMotion(pos, v0, t0);
        if(pos[1]<=below_height){
            jump=false;
            pos[1]=below_height;
            v0 = vector3(0.0,0.0,0.0);
        }
        at[1]=pos[1]+dir1;
	}else{
        //colisão com o chão
        //tmp2 = tmp2 - dot(tmp2,below_floor->getDir())*below_floor->getDir(); //projeção do vetor
        //tmp3 = tmp3 - dot(tmp3,below_floor->getDir())*below_floor->getDir(); //projeção do vetor
        //cout << actual_floor << " " << below_floor << endl;
        if(actual_floor != below_floor){ //queda livre
            //cout << "ok" << endl;
            actual_floor = below_floor;
            jump=true;
            t0 = glutGet(GLUT_ELAPSED_TIME);
        }else{
            pos[1]=below_height;
        }
    }

    if(moveForward) pos += interval*tmp2;
	if(moveBackward) pos -= interval*tmp2;
	if(moveRight) pos -= interval*tmp3;
	if(moveLeft) pos += interval*tmp3;
	at = pos + tmp;
    //cout << collider[1] << endl;

}

Portal* Character::collision(std::vector<vector3>* walls, std::vector<Floor*>* floors){
    Portal* teleport_portal=NULL;
    vector3 ld;
    vector3 hv = vector3(0.0,0.9,0.0);
    vector3 zeros = vector3(0,0,0);
    list<Entity*>::iterator p;
    for (p = entityList->begin(), p++; p!=entityList->end(); p++){
        if (Portal* portal = dynamic_cast<Portal*>(*p)) {
            teleport_portal = portal->teleport_detection(pos+hv);
            if(teleport_portal!=NULL) break;
        }else if (Wall* wall = dynamic_cast<Wall*>(*p)) {
            ld = wall->collision_detection(pos);
            if(ld!=zeros) walls->push_back(ld);
        }
        else if (Floor* floor = dynamic_cast<Floor*>(*p)) {
            ld = floor->collision_detection(pos);
            if(ld!=zeros) floors->push_back(floor);
        }
    }
    return teleport_portal;
}


void Character::drawEntity(){
    glPushMatrix();
    glDepthFunc(GL_ALWAYS);
        glPushMatrix();
            if(takenEntity[1]!=NULL) takenEntity[1]->drawEntity();
        glPopMatrix();
        takenEntity[0]->drawEntity();
        takenEntity[2]->drawEntity();
    glDepthFunc(GL_LESS);
    look();
}

void Character::takeEntity(){
    list<Entity*>::iterator nearest;
    vector3 minDist;
    GLfloat minDistLength = 10000000;
    GLfloat dirLength = dir.length();

    list<Entity*>::iterator p;
    for (p = entityList->begin(), p++; p!=entityList->end(); p++){
        if (!dynamic_cast<Bound*>(*p)){
            if(dynamic_cast<Cube*>(*p)){
                vector3 pos = (*p)->getPos();
                vector3 dist = Character::pos - pos;
                GLfloat distLength = dist.length();
                GLfloat angleCos = (dir[0]*dist[0]+dir[1]*dist[1]+dir[2]*dist[2])/(dirLength*distLength);

                if(angleCos<-0.75 && distLength<3.0 && distLength<minDistLength){
                        nearest = p;
                        takenEntity[1] = *p;
                        minDist = dist;
                        minDistLength = distLength;
                }
            }
        }else break;
    }

    if(takenEntity[1]!=NULL){
        ///TODO
        /*GLfloat angle = ((Cube*)takenEntity)->getAngle();

        vector3 cubeDir = rotate_vector(vector3(0.0,0.0,1.0), vector3(0.0, 1.0, 0.0), angle);
        cout << cubeDir << endl;
        GLfloat cosAlfa = (cubeDir[0]*minDist[0]+cubeDir[1]*minDist[1]+cubeDir[2]*minDist[2])/(minDistLength);
        cout << cosAlfa << endl;
        GLfloat gamma = ((acos(cosAlfa)/3.14)*180.0);
        //if(minDist[0]>0) gamma=-gamma;//<<<<<<<<<<
        cout << gamma << endl << endl;
        ((Cube*)takenEntity)->setAngle(gamma);*/

        takenEntity[1]->setPos(vector3(0.0,-0.5,-2.0));
        entityList->erase(nearest);
    }
}

void Character::releaseEntity(){
    Cube* cube = dynamic_cast<Cube*>(takenEntity[1]);
    ///TODO
    /*GLfloat angle = dir[2]/(dir.length());
    angle = (acos(angle)/3.14)*180.0;
    cout << angle << endl;
    cube->setAngle(angle + cube->getAngle());*/
    cube->setCollide(false);
    cube->setV0(releaseForce*(1/cube->getMass())*0.5);
    takenEntity[1]->setPos(pos+vector3(0.0,1.0+dir1,0.0)+dir*2);
    //cout << takenEntity->getPos() << endl;
    entityList->insert(++(entityList->begin()),takenEntity[1]); //insere depois do primeiro elemento
    takenEntity[1] = NULL;
}

void Character::throwEntity(){
    Cube* cube = dynamic_cast<Cube*>(takenEntity[1]);
    cube->setCollide(false);
    cube->setV0(((shotForce)*vector3(dir[0],dir1,dir[2]))*(1/cube->getMass())*0.5);
    takenEntity[1]->setPos(pos+vector3(0.0,1.4+dir1,0.0)+dir*2);
    entityList->insert(++(entityList->begin()),takenEntity[1]); //insere depois do primeiro elemento
    takenEntity[1] = NULL;
}
