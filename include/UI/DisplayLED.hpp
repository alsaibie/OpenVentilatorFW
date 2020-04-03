#include "board_configuration.hpp"

namespace UI {
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