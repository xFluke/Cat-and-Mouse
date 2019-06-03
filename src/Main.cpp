#include "SDL_Manager.h"
#include "UI_Manager.h"
#include "Game_Manager.h"

#include <SDL_ttf.h>

int main (int argc, char* argv[])
{
    SDL_Manager* pSDL = SDL_Manager::GetInstance();
    
    if (pSDL->Init("Cat and Mouse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 992, 992, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, // SDL_WINDOW_BORDERLESS |
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE) != 0)
    {
        cout << "pSDL->Init failed" << endl;
        return 1;
    }

    UI_Manager pUI;
    Game_Manager pGame;
    

    SDL_SetRenderDrawColor(pSDL->GetRenderer(), 0, 0, 0, 255); // Can it be within another function
    
    while (pSDL->IsRunning())
    {
        pUI.Start(pSDL->GetRenderer(), pSDL->IsRunning(), pGame.IsRunning());
            // Renders Current Screen
            // Input is asked for within Menu
            // Changes current Screen
        
        if (pGame.IsRunning()) // Recieves which level was selected
        {
            cout << "Instance of Game will run" << endl;
            cout << "This is an infinite loop." << endl;
            pGame.Start();
            // Look to pGame->Level1()
            // turn bGameIsRunning back to false after the game loop
        }
        SDL_Delay(20);
    }











    




    //Decontructing
    if (pSDL != nullptr)
    {
        delete pSDL;
    }
    
    return 0;
}
