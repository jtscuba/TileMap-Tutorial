#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos_util.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto layer = HelloWorld::create();

    auto scene = Scene::create();

    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    HANDLE_INIT_ERRORS(Node::init(), "Helloworld Layer failed to initalize\n");

    _map = TMXTiledMap::create("TileMap.tmx");
    _background = _map->getLayer("Background");

    // Add map to layer
    addChild(_map);
    // name from Tiled map
    auto objectGroup = _map->getObjectGroup("Objects");

    HANDLE_INIT_ERRORS(objectGroup, "ObjectGroup pointer is nullptr\n");

    auto spawnPoint = objectGroup->getObject("SpawnPoint");
    int x = spawnPoint.at("x").asInt();
    int y = spawnPoint.at("y").asInt();

    log("x: %d\ty: %d", x, y);

    _player = Sprite::create("Player.png");
    _player->setPosition(Vec2(x,y));

    addChild(_player);
    setViewPointCenter(_player->getPosition());

    //  Create a "one by one" touch event listener
    // (processes one touch at a time)
    // Left-Click is also a touch event
    auto listener1 = EventListenerTouchOneByOne::create();

    // trigger when you push down
    listener1->onTouchBegan = [](Touch* touch, Event* event)
    {
        // your code
        return true; // if you are consuming it
    };

    // trigger when moving touch
    listener1->onTouchMoved = [](Touch* touch, Event* event)
    {
        // your code
        return true;
    };

    // trigger when you let up
    listener1->onTouchEnded = [=](Touch* touch, Event* event){
        // your code
        log("touch ended");

        Vec2 touchLoc = touch->getLocation(); 
        //touchLoc = Director::getInstance()->convertToGL(touchLoc); getLocation returns GL coords so I don;t think this is needed.
        touchLoc = this->convertToNodeSpace(touchLoc); // change from GL coords to coords relative to this node
        
        Vec2 playerPos = _player->getPosition();
        Vec2 diff = touchLoc - playerPos;

        const float scaleFactor = Director::getInstance()->getContentScaleFactor();

        if (abs(diff.x) > abs(diff.y))
        {
            playerPos.x += sign(diff.x) * _map->getTileSize().width / scaleFactor;
        }
        else
        {
            playerPos.y += sign(diff.y) * _map->getTileSize().height / scaleFactor;
        }

        const int MAP_WIDTH_IN_PX = _map->getMapSize().width * _map->getTileSize().width / scaleFactor;
        const int MAP_HEIGHT_IN_PX = _map->getMapSize().height * _map->getTileSize().height / scaleFactor;

        if (playerPos.x >=0 
            and playerPos.y >= 0 
            and playerPos.x <= MAP_WIDTH_IN_PX 
            and playerPos.y <= MAP_HEIGHT_IN_PX)
        {
            _player->setPosition(playerPos);
            this->setViewPointCenter(playerPos);
        }
        return true;
    };

    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    return true;
}

void HelloWorld::setViewPointCenter(cocos2d::Vec2 position)
{
    auto winSize = Director::getInstance()->getWinSize();

    const float scaleFactor = Director::getInstance()->getContentScaleFactor();

    const int MAP_WIDTH_IN_PX = _map->getMapSize().width * _map->getTileSize().width / scaleFactor;
    const int MAP_HEIGHT_IN_PX = _map->getMapSize().height * _map->getTileSize().height / scaleFactor;

    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y , winSize.height/2);

    x = MIN(x, MAP_WIDTH_IN_PX - winSize.width/2);
    y = MIN(y, MAP_HEIGHT_IN_PX - winSize.height/2);

    Vec2 actualPosition = Vec2(x,y);

    Vec2 centerOfView = Vec2(winSize.width/2, winSize.height/2);
    Vec2 newMapPosition = centerOfView - actualPosition;
    setPosition(newMapPosition);

}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
