#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

class ScaledSpriteCompare {
public:
  bool operator() (Drawable * lhs, Drawable * rhs) {
    return (static_cast<ScaledSprite*>(lhs)->getScale() < static_cast<ScaledSprite*>(rhs)->getScale());
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  if(explodingScaledSprite.size() != 0 ){
    std::list<ExplodingSprite*>::iterator itr = 
    explodingScaledSprite.begin();
    while( itr != explodingScaledSprite.end()){
      delete *itr;
      itr = explodingScaledSprite.erase(itr);
    }
  }
  sprites.clear();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  backBuilding("backBuilding", Gamedata::getInstance().getXmlInt("backBuilding/factor") ),
  frontBuilding("frontBuilding", Gamedata::getInstance().getXmlInt("frontBuilding/factor") ),
  poles("poles", Gamedata::getInstance().getXmlInt("poles/factor") ),
  road("road", Gamedata::getInstance().getXmlInt("road/factor") ),
  viewport( Viewport::getInstance() ),
  hudFlag(false),
  hud(),
  healthBar(),
  sprites(),
  explodingScaledSprite(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  numberOfRockets(Gamedata::getInstance().getXmlInt("rocket/numberOfRockets")),
  score(0),
  healthRemaining(true),
  sound(),
  godMode(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back(new Player("player"));
  makeScaledRockets();
  viewport.setObjectToTrack(sprites[0]);
}

void Manager::makeScaledRockets() {
  for (unsigned i = 0; i < numberOfRockets; ++i){
    sprites.push_back(new ScaledSprite("rocket"));
  }
  sort(sprites.begin()+1 , sprites.end() , ScaledSpriteCompare());
}

void Manager::draw() const {
  backBuilding.draw();
  for (unsigned i = 1; i < sprites.size()/2; ++i){
    sprites[i]->draw();
  }
  frontBuilding.draw();
  poles.draw();
  road.draw();
  for (unsigned i = sprites.size()/2; i < sprites.size(); ++i){
    sprites[i]->draw();
  }
  std::list<ExplodingSprite*>::const_iterator itr = explodingScaledSprite.begin();
  while(itr != explodingScaledSprite.end() ){
    (*itr)->draw();
    ++itr;
  }
  if(healthRemaining) sprites[0]->draw();
  io.printMessageAt(title, 10, 570);
  if(!healthRemaining) {
    clock.pause();
    io.printMessageValueAt("Your final score is ", score, 360, 420);
  }
  viewport.draw();
  io.printMessageValueAt("Your score is ", score , 400 , 570);
  drawHUD();
  SDL_Flip(screen);
}

void Manager::drawHUD() const {
  if(clock.getSeconds() <3){
    hud.drawHUD(screen, io, clock.getSeconds(), clock.getFps(), static_cast<Player*>(sprites[0])->getBulletListSize() , static_cast<Player*>(sprites[0])->getFreeListSize());
   }
  else if(hudFlag==true) {
    hud.drawHUD(screen, io, clock.getSeconds(), clock.getFps(), static_cast<Player*>(sprites[0])->getBulletListSize(), static_cast<Player*>(sprites[0])->getFreeListSize());
  }
  healthBar.draw();
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}


void Manager::update() {
  clock.update();
  backBuilding.update();
  frontBuilding.update();
  poles.update();
  road.update();
  io.printMessageValueAt("Your score is ", score , 10 , 600);
  Uint32 ticks = clock.getTicksSinceLastFrame();

  std::list<ExplodingSprite*>::iterator itr = explodingScaledSprite.begin();
  while( itr != explodingScaledSprite.end()){
    (*itr)->update(ticks);
    if((*itr)->done()){
      delete *itr;
      itr = explodingScaledSprite.erase(itr);
    }
    else ++itr;
  }

  for (unsigned int i = 1; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }

  std::vector<Drawable*>::iterator ptr = sprites.begin() + 1;
  while(ptr != sprites.end() && healthRemaining){
    if(static_cast<Player*>(sprites[0])->collidWith(*ptr) ){
      explodingScaledSprite.push_back(new ExplodingSprite(*(static_cast<ScaledSprite*>(*ptr))));
      ScaledSprite* player = static_cast<ScaledSprite*>( sprites[0] );
      ScaledSprite* sprite = static_cast<ScaledSprite*>( *ptr );      
      *ptr = new SmartSprite(sprite->getName(), sprite->getPosition() + Vector2f(100,100), *player); 
      delete sprite;
      ptr++;      
      ++score;  
      sound[1];                                               
    }
    else ++ptr;
  }

  std::vector<Drawable*>::iterator pos = sprites.begin() + 1;
  while(pos != sprites.end()){
    (*pos)->update(ticks);
    if(static_cast<Player*>(sprites[0])->playerCollidWith(*pos) && healthRemaining){
      score = score + 5;
      explodingScaledSprite.push_back(new ExplodingSprite(*(static_cast<ScaledSprite*>(*pos))));
      ScaledSprite* player = static_cast<ScaledSprite*>(sprites[0]);
      ScaledSprite* sprite = static_cast<ScaledSprite*>(*pos);      
      *pos = new SmartSprite(sprite->getName(), sprite->getPosition() + Vector2f(100,100), *player); 
      delete sprite;
      pos++;
      sound[1];
    }
    else ++pos;
  }
  
  if (makeVideo && frameCount < frameMax) makeFrame();
  if(godMode) healthBar.reset();
  healthBar.update(ticks , godMode);
  if(healthBar.getCurrentHealth() == 0) healthRemaining = false;  
  if(healthRemaining) sprites[0]->update(ticks);
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;
  clock.start();

  while ( not done ) {
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }
      if ( keystate[SDLK_p] && !keyCatch) {
        keyCatch = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
      }
      if ( keystate[SDLK_f]  && !keyCatch) {
        keyCatch = true;
        clock.toggleCapFrame();
      }
      if (keystate[SDLK_m]  && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
      if (keystate[SDLK_F1]  && !keyCatch) {
        keyCatch =true;
        hudFlag = !hudFlag;
      }
      if (keystate[SDLK_g]  && !keyCatch) {
        keyCatch =true;
        godMode = !godMode;
      }
      if (keystate[SDLK_r]  && !keyCatch) {
        keyCatch = true;
        healthRemaining = true;
        healthBar.reset();
        score = 0;
        clock.unpause();
        viewport.setObjectToTrack(sprites[0]);
        std::vector<Drawable*>::iterator itr = sprites.begin();
        while(itr != sprites.end()){
          delete *itr;
          itr = sprites.erase(itr);
        }
        sprites.push_back(new Player("player"));
      	for(unsigned i = 0; i < numberOfRockets; i++){
    	  sprites.push_back( new ScaledSprite("rocket") );
  	}
  	sort(sprites.begin()+1, sprites.end(),ScaledSpriteCompare());
  	viewport.setObjectToTrack(sprites[0]);   
      }
      if (keystate[SDLK_SPACE]) {
        sound[0];
        static_cast<Player*>(sprites[0])->shoot(false);
      }
      if (keystate[SDLK_SPACE] and keystate[SDLK_d]) {
        sound[0];
        static_cast<Player*>(sprites[0])->shoot(true);
      }

      if (keystate[SDLK_a]) {
        static_cast<Player*>(sprites[0])->left();
      }
      if (keystate[SDLK_d]) {
        static_cast<Player*>(sprites[0])->right();
      }
      if (keystate[SDLK_w]) {
        static_cast<Player*>(sprites[0])->up();
      }
      if (keystate[SDLK_s]) {
        static_cast<Player*>(sprites[0])->down();
      }
      if (keystate[SDLK_a] && keystate[SDLK_d]) {
        static_cast<Player*>(sprites[0])->stop();
      }  
      if (keystate[SDLK_w] && keystate[SDLK_s]) {
        static_cast<Player*>(sprites[0])->stop();
      }
      if (keystate[SDLK_F4] && !makeVideo  && !keyCatch) {
        keyCatch = true;
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }
    }    
    draw();
    update();
  }
}
