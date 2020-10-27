// Based on OpenAvTutorials , Thanks Harry !!

#include <stdio.h>
#include <unistd.h>
#include <jack/jack.h>
#include <math.h>
#include <sndfile.h>
#include <malloc.h>
#include <jack/midiport.h>
#include <string.h>
//#include "Midi.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>

// declare two "jack_port_t" pointers, which will each represent a port
// in the JACK graph (ie: Connections tab in QJackCtl)
jack_port_t* inputPort = 0;
jack_port_t* outputPort = 0;
// this function is the main audio processing loop, JACK calls this function
// every time that it wants "nframes" number of frames to be processed.
// nframes is usually between 64 and 256, but you should always program
// so that you can work with any amount of frames per process() call!


int process(jack_nframes_t nframes, void* emptyshell)
{
  // this is a little tricky, port_get_buffer() will return a pointer to
  // the data that we will use, so cast it to (float*), so that we
  // can use the data as floating point numbers. JACK will always pass
  // floating point samples around, the reason that we have to cast it
  // ourselves is so that it could be changed in the future... don't worry
  // about it too much.
  jack_midi_event_t in_event;

  float* inputBuffer = (float*)jack_port_get_buffer ( inputPort , nframes);
  float* outputBuffer= (float*)jack_port_get_buffer ( outputPort, nframes);

// Creation d'une variable globale pour gérer les evenements SDL a l'intérieur de process et de main
SDL_Event event;


    while( SDL_PollEvent(&event) != 0)
    {
        jack_midi_clear_buffer(outputBuffer);
        //printf("Damage : %s\n", SDL_GetError());
        unsigned char* buffer = jack_midi_event_reserve (outputBuffer, 0, 3);
        switch(event.type) {
            case SDL_QUIT:
                break;
            case SDL_CONTROLLERAXISMOTION:
                printf("Axis has been moved");
                switch(event.caxis.axis) {
                    case SDL_CONTROLLER_AXIS_LEFTX:
                        printf("Left Axis  at horizontal position of : %i", event.caxis.value);
                        break;
                    case SDL_CONTROLLER_AXIS_LEFTY:
                        printf("Left Axis at vertical position of  : %i", event.caxis.value);
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTX:
                        printf("Right Axis at horizontal position of : %i", event.caxis.value);
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTY:
                        printf("Right Axis at vertical position of  : %i", event.caxis.value);
                        break;
                }

                break;
            case SDL_CONTROLLERBUTTONDOWN:
                switch(event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_A:
                        if(event.cbutton.state == SDL_PRESSED) {
                        printf("Button A pressed!");
                        buffer[0] = 177;
                        buffer[1] = 9; 
                        buffer[2] = 127;
                        break;}
                    case SDL_CONTROLLER_BUTTON_B:
                        if(event.cbutton.state == SDL_PRESSED) {
                        printf("Button B pressed!");
                        buffer[0] = 177;
                        buffer[1] = 9; 
                        buffer[2] = 95;
                        break;
                }
                    case SDL_CONTROLLER_BUTTON_X:
                        if(event.cbutton.state == SDL_PRESSED) {
                        printf("Button X pressed!");
                        buffer[0] = 177;
                        buffer[1] = 9; 
                        buffer[2] = 63;
                        break;
                        }
                    case SDL_CONTROLLER_BUTTON_Y:
                    printf("Button Y pressed!");
                        if(event.cbutton.state == SDL_PRESSED) {
                        buffer[0] = 177;
                        buffer[1] = 9; 
                        buffer[2] = 27;
                        break;
                        }
                    case SDL_CONTROLLER_BUTTON_START:
                        printf("Button Start pressed!");
	                    SDL_Quit();
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        printf("RightShoulder pressed!");
                        if(event.cbutton.state == SDL_PRESSED) {
                        buffer[0] = 176;
                        buffer[1] = 8; 
                        buffer[2] = 127;
                        break;}
                }
            case SDL_CONTROLLERBUTTONUP:
                switch(event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_A:
                        printf("Button A released!");
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        printf("Button B pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_X:
                        printf("Button X pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        printf("Button Y pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_START:
                        printf("Button Start pressed!");
	                    SDL_Quit();
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        printf("RightShoulder released!");
                        if(event.cbutton.state == SDL_RELEASED && event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
                        buffer[0] = 176;
                        buffer[1] = 8; 
                        buffer[2] = 0;
                        break;}
    }
        }
        
        printf("...");

    
    }



  //jack_nframes_t event_count = jack_midi_get_event_count(inputBuffer);
  //jack_midi_event_write(outputBuffer,nframes ,in_event.buffer ,nframes );
  // one by one, copying them across. Try multiplying the input by 0.5,
  // it will decrease the volume...
  /**
  struct Midi_Event test;
  for ( int i = 0; i < (int) nframes; i++)
  {
    if(event_count > 0)
    {
    jack_midi_event_get(&in_event, inputBuffer, i);
    printf("Raw shit : %i , %i , %i", in_event.buffer[0], in_event.buffer[1], in_event.buffer[2]);
    test = DataToMidiEvent(in_event);
    //printf("Type  %s, Channel : %i , Note : %i, Velocity : %i \n", test.type, test.channel, test.note, test.velocity); 


    //Ecriture sur le outputBuffer du buffer lu en input
    
    unsigned char* buffer = jack_midi_event_reserve ( outputBuffer, 0, 3);
    buffer[0] = in_event.buffer[0];
    buffer[1] = in_event.buffer[1];
    buffer[2] = in_event.buffer[2];
    
    //MidiTrigger(test); 
    }
    event_count = 0;
 
  } 
       
    **/
  
  return 0;
}





int main(int argc, char * argv[])
{
	if ( SDL_Init(SDL_INIT_GAMECONTROLLER |  SDL_INIT_EVENTS) != 0)
        {
            SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
           return 1;
        }
        ;
	/* Open the first available controller. */
	char *mapping;
SDL_GameController *controller = NULL;
for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    if (SDL_IsGameController(i)) {
        controller = SDL_GameControllerOpen(i);
        if (controller) {
		mapping = SDL_GameControllerMapping(controller);
		printf("\nMapping is such as follows : %s", mapping);
            break;
        } else {
            fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
        }
    }
}
  printf("\n **Test GamePad to Midi** \n");

  // create a JACK client and activate
  jack_client_t* client = jack_client_open ("GamepadToMidi",
                                            JackNullOption,
                                            0,
                                            0 );
  printf("\n *Jack Client Created\n");

  // register the process callback : JACK "calls us back" when there is
  // work to be done, and the "process" function does that work.
  jack_set_process_callback  (client, process , 0);
  
  // register two ports, one input one output, both of AUDIO type
  inputPort = jack_port_register ( client,
                                    "input",
                                    JACK_DEFAULT_MIDI_TYPE,
                                    JackPortIsInput,
                                    0 );
  
  outputPort = jack_port_register ( client,
                                    "output",
                                    JACK_DEFAULT_MIDI_TYPE,
                                    JackPortIsOutput,
                                    0 );
  
  // activate the client, ie: enable it for processing
  jack_activate(client);
    /**
    int isrunning = 1;
    //SDL_Event event;
    while(isrunning == 1 )
    {
    while( SDL_PollEvent(&event) != 0)
    {
        //printf("Damage : %s\n", SDL_GetError());
        switch(event.type) {
            case SDL_QUIT:
                isrunning=0;
                break;
            case SDL_CONTROLLERAXISMOTION:
                printf("Axis has been moved");
                switch(event.caxis.axis) {
                    case SDL_CONTROLLER_AXIS_LEFTX:
                        printf("Left Axis  at horizontal position of : %i", event.caxis.value);
                        break;
                    case SDL_CONTROLLER_AXIS_LEFTY:
                        printf("Left Axis at vertical position of  : %i", event.caxis.value);
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTX:
                        printf("Right Axis at horizontal position of : %i", event.caxis.value);
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTY:
                        printf("Right Axis at vertical position of  : %i", event.caxis.value);
                        break;
                }

                break;
            case SDL_CONTROLLERBUTTONDOWN:
                switch(event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_A:
                        printf("Button A pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        printf("Button B pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_X:
                        printf("Button X pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        printf("Button Y pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        printf("Button Left pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        printf("Button Right pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        printf("Button Up pressed !");
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        printf("Button Down pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_START:
                        printf("Button Start pressed!");
                        isrunning=0;
	                    SDL_Quit();
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                        printf("Leftstick pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                        printf("Right stick pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                        printf("LeftShoulder pressed");
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        printf("RightShoulder pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_GUIDE:
                        printf("Whatever the fuck a guide is is now pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_BACK:
                        printf("Back Button pressed !");
                        break;
                    case SDL_CONTROLLER_BUTTON_MAX:
                        printf("Button Max pressed!");
                        break;
                    case SDL_CONTROLLER_BUTTON_INVALID:
                        printf("FUCK YOU PRESSED ?! Nigga");
                        break;
                }
    }
        printf("...");

    }
   } 

**/
  // pause for 30 seconds, letting process() do it's thing
  sleep(30000);
  
  // tell JACK to stop processing the client
  jack_deactivate(client);
  
  // close the client
  jack_client_close(client);
  
  return 0;
    
}

