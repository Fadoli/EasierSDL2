#ifndef font_H
#define font_H

#include <stdio.h>
#include "lib_core.h"
#include "lib_screen.h"

namespace lib {
class font
{
    public:
        font( lib_Screen * , const char * FontName )
        {
            int FontSize;
			char FontFile[512];
            sscanf(FontName,"%s %d",FontFile,&FontSize);
            self = TTF_OpenFont(FontFile,FontSize);
        }
        font( lib_Screen * , const char * FontFile, int FontSize )
        {
            int FontSize;
            self = TTF_OpenFont(FontFile,FontSize);
        }

        void Free ()
        {
            if (self)
                TTF_CloseFont(self);
            self = NULL;
        }

        ~font()
        {
            Free ();
        }

        TTF_Font * getFont ()
        {
            return self;
        }

    private:
        TTF_Font * self;
};
}

#endif // font_H
