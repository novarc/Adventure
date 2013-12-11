/*
 * GameMain.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"
#include "EntityMap.hpp"
#include "PhysicsMap.hpp"
#include "SideScrollingView.hpp"

class StaticColoredBox : public Entity
{
public:
    StaticColoredBox(xy pos, xy sz) : Entity(new ColoredBox(sz), pos) {}
};

DynamicEntityCharacteristics pivotDynaChars(
        1, // groundFriction
        1, // gravityFactor
        12, // maxHorizontalSpeed
        2, // horizontalWalkStep
        25 // jumpStep
        );

class DynamicColoredBox : public DynamicEntity
{
public:
    DynamicColoredBox(xy pos, xy sz, const DynamicEntityCharacteristics &dynamicChars) :
        DynamicEntity(new ColoredBox(sz), pos, dynamicChars) {}
};

class Steppable
{
public:
    virtual void step() = 0;
    virtual ~Steppable() {}
};

class TestBed : public Steppable, public InputCallbacks
{
    SideScrollingView sideScrollingView;
    PhysicsMap physicsMap;
    vector<StaticColoredBox> boxes;
    DynamicEntity *pivot;

public:
    TestBed() : physicsMap( xy(1024, 600), 32 )
    {
        boxes.push_back(StaticColoredBox( xy(1, 1) , xy(200, 100) ));
        boxes.push_back(StaticColoredBox( xy(270, 140) , xy(100, 20) ));
        boxes.push_back(StaticColoredBox( xy(300, 290) , xy(100, 100) ));
        boxes.push_back(StaticColoredBox( xy(500, 400) , xy(400, 100) ));
        pivot = new DynamicColoredBox( xy(100, 170) , xy(25, 25), pivotDynaChars );

        set<Entity *> collidingEntities;
        for(auto &box : boxes)
            physicsMap.place(&box, collidingEntities);

        physicsMap.place(pivot, collidingEntities);

        sideScrollingView.physicsMap = &physicsMap;
        sideScrollingView.pivot = pivot;
    }

    void step()
    {
        Sys()->getInput(this);

        physicsMap.performPhysics();

        sideScrollingView.render();

        stringstream ss;
        std::function<void (Entity*)> sscat = [&ss](Entity *e) {
            Rect rect = e->getRect();
            xy exterm = rect.pos + rect.size;
            ss<<" is at "<<rect.pos<<" and has size "<<rect.size<<
                    " and its upper-right corner is at "<<exterm<<"\n";
        };

        for(int i = 0; i < boxes.size(); i++) {
            ss<<"Box["<<i<<"]";
            sscat(&boxes[i]);
        }
        ss<<"Pivot";
        sscat(pivot);
        Sys()->drawText(ss.str(), xy(10,10));
    }

    void escKey() { Sys()->exit(); }

    void upKey() { physicsMap.jump(pivot); }
    void leftKey() { physicsMap.walkLeft(pivot); }
    void rightKey() { physicsMap.walkRight(pivot); }
};

class OldGameMap : public Steppable, public InputCallbacks
{
    DynamicEntity *pivot;
    unique_ptr<PhysicsMap> physicsMap;
    SideScrollingView ssv;

public:
    OldGameMap() : pivot(nullptr)
    {
        // get images:
        map<const string, shared_ptr<Tex>> images;

        ifstream img_files("images.txt");

        if(img_files.fail())
            throw invalid_argument("Unable to open image list file.");

        string s;

        while(!img_files.eof())
        {
            string image_name, file_name;

            getline(img_files, image_name, ',');
            getline(img_files, file_name);

            images[image_name] = Sys()->loadTex(file_name);
        }

        img_files.close();

        // load level

        ifstream level_file("level.txt");

        if(level_file.fail())
            throw invalid_argument("Unable to open level file.");

        xy size;

        string d;
        getline(level_file, d);
        size.x = atoi(d.c_str());
        getline(level_file, d);
        size.y = atoi(d.c_str());

        physicsMap = move( unique_ptr<PhysicsMap>( new PhysicsMap(size, 32) ) );

        set<Entity *> collidingEntities;

        while(!level_file.eof())
        {
            string s;
            xy p;

            getline(level_file, s, ',');

            if(level_file.eof())
                break;

            getline(level_file, d, ',');
            p.x = atoi(d.c_str());
            getline(level_file, d);
            p.y = atoi(d.c_str());

            //DrawableAABB *d = new Image( images.at(s) );
            DrawableAABB *d = new ColoredBox( images.at(s)->getSize() );

            Entity *e = nullptr;

            if(s == "Player") {
                pivot = new DynamicEntity(d, p, pivotDynaChars);
                e = pivot;
            }
            else
                e = new Entity(d, p);

            if( !physicsMap->place(e, collidingEntities) ) {
                cerr<<"Collision"<<endl;
                break;
            }
        }

        level_file.close();

        // setup scv
        ssv.physicsMap = &*physicsMap;
        ssv.pivot = pivot;
    }

    void escKey() { Sys()->exit(); }

    void upKey() { physicsMap->jump(pivot); }
    void leftKey() { physicsMap->walkLeft(pivot); }
    void rightKey() { physicsMap->walkRight(pivot); }

    void step() {
        Sys()->getInput(this);
        physicsMap->performPhysics();
        ssv.render();
    }
};

GameMain* GameMain::singleton = nullptr;

class GameMainImpl : public GameMain
{
    unique_ptr<Steppable> whatever;

public:
    GameMainImpl()
    {
        whatever = unique_ptr<Steppable>( new TestBed() );
        //whatever = unique_ptr<Steppable>( new OldGameMap() );
    }

    void step() {
        whatever->step();
    }

    ~GameMainImpl() {
    }
};

WindowProperties GameMain::defaultWindowProperties()
{
    WindowProperties windowProperties;

    // Default window height & width:
    windowProperties.size = xy(1024, 600);
    windowProperties.fullscreen = false;

    // Title
    windowProperties.title = "Adventure";

    return windowProperties;
}

GameMain* GameMain::getSingleton() {
    if( GameMain::singleton == nullptr )
        GameMain::singleton = new GameMainImpl();
    return GameMain::singleton;
}
