#ifndef LIB_OBJECT_H
#define LIB_OBJECT_H


#define INVALID (-1)

// English
/// This class is the base class that should be used in all others drawable classes
/// it does affect images, buttons ...
// French
/// Cette classe est une classe de base qui seras a implementer dans toutes les classes affichable
/// a savoir, cela affecte les images, boutons ...

class lib_Object
{
public:
    /** Default constructor */
    lib_Object( );
    /** Default destructor */
    virtual ~lib_Object();

    virtual void setZoomx( double Zoom ) = 0;
    virtual void setZoomy( double Zoom ) = 0;
    virtual void setZoom( double Zoom ) = 0;
    virtual void setZoom( double Zoomx, double Zoomy ) = 0;
    virtual void setSize( int w, int h ) = 0;

    virtual void setCenter ( bool t = true ) = 0;
    virtual void draw   ( int x, int y, int Ang = 0 ) = 0;
protected:
private:
};

#endif // lib_Object_H
