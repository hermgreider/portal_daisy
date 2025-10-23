#include "External/board_input.h"
#include "External/pod_input.h"
#include "External/simple_fix_input.h"
#include "External/seed_input.h"
#include "board_factory.h"

// TODO: Probably eliminate this class

SeedInput *BoardFactory::CreateBoard(Config *config)
{
    return new SeedInput();
    // switch (config->boardType) 
    // {
    //     case BoardType::POD:
    //         return new PodInput();
    //         break;
    //     case BoardType::SIMPLEFIX:
    //         return new SimpleFixInput();
    //         break;
    //     case BoardType::SEED:
    //         return new SeedInput();
    //         break;
    // }
    // return NULL;
}
