#ifndef OBJECT_H
#define OBJECT_H

// English
/// This class is the base class that should be used in all others drawable classes
/// it does affect images, buttons ...
// French
/// Cette classe est une classe de base qui seras a implementer dans toutes les classes affichable
/// a savoir, cela affecte les images, boutons ...

namespace sdl2_lib
{
/**
 *  \brief Flip constants
 */
typedef enum
{
    FLIP_NONE = 0x00000000,       // No flip
    FLIP_HORIZONTAL = 0x00000001, // Horizontal flip
    FLIP_VERTICAL = 0x00000002    // Vertical flip
} Flip;

/**
 * @brief Base object for rendering / position logics
 */
class Object
{
public:
    /** Default constructor */
    Object(double w = 100, double h = 100, double angle = 0);
    virtual void Reset(double w = 100, double h = 100, double angle = 0);
    /** Default destructor */
    virtual ~Object();

    /**
     * @brief Set the Zoom object
     * 
     * @param Zoomx 
     * @param Zoomy 
     */
    virtual void setZoom(double Zoomx, double Zoomy);
    /**
     * @brief Set the Size object
     * 
     * @param w 
     * @param h 
     */
    virtual void setSize(double w, double h);
    /**
     * @brief Set the Center object
     * 
     * @param centered 
     */
    virtual void setCenter(bool centered = true);
    /**
     * @brief Set the Pos object
     * 
     * @param x 
     * @param y 
     */
    virtual void setPos(double x, double y);
    /**
     * @brief Set the Ang object
     * 
     * @param ang
     */
    virtual void setAng(double ang);

    /**
     * @brief Start a 'simple' draw command
     */
    virtual void drawAt(double posx, double posy, double ang = 0, Flip FLIP = Flip::FLIP_NONE);
    /**
     * @brief Start the draw command
     */
    virtual void draw(Flip FLIP = Flip::FLIP_NONE);
    /**
     * @brief Start the draw command
     */
    virtual void draw(int ox, int oy, Flip FLIP = Flip::FLIP_NONE);

protected:
    /**
     * @brief Actual render command : needs to be implemented by objects
     * 
     * @param x 
     * @param y 
     * @param Ang 
     * @param FLIP 
     */
    virtual void internal_draw(int x, int y, double Ang, Flip FLIP) = 0;
    /**
     * @brief Actual render command : needs to be implemented by objects
     * 
     * @param x 
     * @param y 
     * @param Ox 
     * @param Oy 
     * @param Ang 
     * @param FLIP 
     */
    virtual void internal_draw(int x, int y, int Ox, int Oy, double Ang, Flip FLIP) = 0;

    double posx = 0, posy = 0;
    double deltax = 0, deltay = 0;
    double zoomx = 1, zoomy = 1;
    double w = -1, h = -1, ang = 0;
    double base_w = -1, base_h = -1, base_ang = 0;
    bool isCentered = false;

private:
    /**
     * @brief Internal function to update the computed w/h size from the base size and the current zoom
     */
    void updateSize();
};
} // namespace sdl2_lib

#endif // Object_H
