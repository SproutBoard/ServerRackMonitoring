#include <Time.h>
#include <Bounce.h>
#include <SPI.h>
#include <Ethernet.h>

class ReedSwitches
{
    public:
    ReedSwitches();
    bool Poll();
    void AddPin( int pin );
    void RemovePin( int pin );
    void ClearPins();
    void RestoreFromEeprom( int address );
    void StoreToEeprom( int address );
    void PrintSerial();
    void PrintWeb(EthernetClient client );
//    char *AsXML( char *buffer );
//    char *XMLStatus( char *buffer );
//    char *TRStatus( char *buffer );

    bool _alert;
    int _locationID;
    time_t _incidentTime;

    private:
    int _switchList[ 10 ];
    Bounce *_BounceList [ 10 ];
    int _switchCount;
};


