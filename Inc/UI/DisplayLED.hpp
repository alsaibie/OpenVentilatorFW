#include "board_configuration.hpp"

namespace UI {
typedef enum{
MAINSCREEN = 0,
ASCREEN, 
BSCREEN,

NUM_OF_SCREENS // TODO: Name the remaining screens
}DispalyScreen_t;

struct DisplayLEDData {
    /* Structure Display */
};

class DisplayLED {
   public:
    DisplayLED();

    void Begin();

    void Update(DisplayLEDData &disp_data);

   private:
    /* Parameters */
};
}  // namespace UI