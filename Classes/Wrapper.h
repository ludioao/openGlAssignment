#ifndef __WRAPPER_H__
#define __WRAPPER_H__

class Wrapper 
{
    private:
        int shapeNumber;
        // returns the last shaper number
        int getShapeNumber() const;

    public:
        void renderScene(LPVOID lpParam);
        Wrapper();

};

#endif