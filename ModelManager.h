/* 
 * File:   ModelManager.h
 * Author: bdi_usr
 *
 * Created on 10 de mayo de 2012, 4:10
 */

#ifndef MODELMANAGER_H
#define	MODELMANAGER_H


#include "Modelo.h"

#include <map>

#include <stdlib.h>

class ModelManager {
public:
    
    static ModelManager* Inst();
    
    bool LoadMesh(const char* path,const unsigned int texID);

    bool Render(const unsigned int texID);
    
    Modelo* GetModelo(const unsigned int texID);
    
    virtual ~ModelManager();

private:
    
    ModelManager();
    
    static ModelManager* m_inst;
    
    std::map<unsigned int, Modelo*> m_texID;
    
    Modelo* loadasset(const char* path);

};

#endif	/* MODELMANAGER_H */

