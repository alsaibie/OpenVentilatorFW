#include "board_configuration.hpp"


struct DisplayLEDData{
    /* Structure Display */
};

class DisplayLED{
    public:
    DisplayLED();

    void Begin();

    void Update(DisplayLEDData &disp_data);

  private:
  /* Parameters */

};