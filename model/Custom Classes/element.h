//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_ELEMENT_H
#define PROJECTDA_ELEMENT_H


class Element{

    private:
        int elementId = 0;

    public:

        int getElementId(){
            return elementId;
        }

        void setElementId(int i){
            elementId = i;
        }
};

#endif //PROJECTDA_ELEMENT_H
