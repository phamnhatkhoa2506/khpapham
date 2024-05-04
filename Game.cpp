#include <bits/stdc++.h>
using namespace std;


/// Genenal Previous Declaration
enum ElementType {PATH, WALL, FAKE_WALL};
enum RobotType {C = 0, S, W, SW};
enum ItemType {MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD};

int dis[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

// Functions in head


// Classes in head
class MapElement;
class Path;
class Wall;
class FakeWall;
class Position;
class Map;
class Configuration;

int Mahattan(const Position& a, const Position& b);

class MovingObject;
class Character;

class SherLock;
class Watson;

class Criminal;
class Robot;
class RobotC;
class RobotS;
class RobotW;
class RobotSW;

class ArrayMovingObject;
class StudyInPinkProgram; // chưa xong

class BaseItem;
class MagicBook;
class EnergyDrink;
class FirstAid;
class ExcemptionCard; // chưa xong
class PassingCard; // chưa xong

class BaseBag; 
class SherlockBag; // chưa xong
class WatsonBag; // chưa xong

class TestStudyInPink; // test case



// Classes......................
class MapElement // MapElement
{
	friend class TestStudyInPink;

	public :
		MapElement() {};
		MapElement(ElementType in_type) : type(in_type) {}
		virtual ~MapElement() {}; 
		
		virtual ElementType getType() const = 0;
		
		void setType(ElementType type) // set type
		{ this->type = type; }

	protected :
		ElementType type; // Type of elmeent in the map 
};

class Path : public MapElement // Path
{
	friend class TestStudyInPink;

    public :
        Path() { this->type = PATH; }
		~Path() {}

		ElementType getType() const override { return type; }
};

class Wall : public MapElement // Wall
{
	friend class TestStudyInPink;

    public :
        Wall() { this->type = WALL; }
		~Wall() {}

		ElementType getType() const override { return type; }
};

class FakeWall : public MapElement // Fake wall
{
	friend class TestStudyInPink;

    public :
        FakeWall() { this->type = FAKE_WALL; }
		FakeWall(int in_reqEXP) : reqEXP(in_reqEXP) { this->type = FAKE_WALL; }
		~FakeWall() {}
		
		int getReqExp() const { return reqEXP; }
		ElementType getType() const override { return type; }

    private :
        int reqEXP; // experience of fake wall
};

class Position // Position of character and robot
{
	friend class TestStudyInPink;

    public :
        Position() {}
		Position(int r, int c) : row(r), col(c) {} 
		Position(const string& str_pos) // Read string and convert -> (x, y) : row(x), col(y)
		{
		    string temp;
		    int i = 1;
		    while (isdigit(str_pos[i])) temp += str_pos[i++];
		    row = stoi(temp);
		    temp = "";
		    i++;
		    while (isdigit(str_pos[i])) temp += str_pos[i++];
		    col = stoi(temp);
		}
		Position(const Position& pos) : row(pos.getRow()), col(pos.getCol()) {}
		
		int getRow() const { return row; }
		int getCol() const { return col; }
		void setRow(int r) { row = r; }
		void setCol(int c) { col = c; }
		string str() const { return "(" + to_string(row) + "," + to_string(col) + ")"; }
		bool isEqual(int in_r, int in_c) const { return (in_r == row) and (in_c == col); } // Compare two position
		bool isEqual(const Position& otherPos)
		{
			if (row == otherPos.getRow() and col == otherPos.getCol()) return true;
			return false;
		} 

		Position& operator = (const Position& pos)
		{
		    this->row = pos.getRow();
		    this->col = pos.getCol();
		
		    return *this;
		}
		bool operator == (const Position& pos)
		{
		    return (this->row == pos.getRow()) and (this->col == pos.getCol());
		}
		bool operator != (const Position& pos)
		{
		    return (this->row != pos.getRow()) or (this->col != pos.getCol());
		}

        static const Position npos;
    private :
        int row; // At row ..
        int col; // At column ...
           
};
const Position Position::npos = Position(-1, -1); // Position invalid

class MovingObject // Derived class of Character class ans Robot class
{
	friend class TestStudyInPink;

    public :
        MovingObject() {}
		MovingObject(int index, const Position& pos, Map *map, const string& name = "")
		{
		    this->index = index;
		    this->name = name;
		    this->pos.setRow(pos.getRow());
		    this->pos.setCol(pos.getCol());
		    this->map = map;
		}
		virtual ~MovingObject() {}
        
        virtual Position getNextPosition() = 0;
        virtual Position getCurrentPosition() const = 0;
        virtual void move() = 0;
        virtual string str() const = 0;

        int getIndex() const { return index; }
        Position getPosition() const { return pos; }
		string getName() const { return name; }
		void setIndex(int index) { this->index = index; }
		void setPosition(const Position& pos) { this->pos = pos; }
		void setName(string name) { this->name = name; }

    protected :
        int index; // index in "moving object array"
        Position pos; // Position of object at current time
        Map *map; // Pointer of the map 
        string name; // Name of the object

};

class Map // Map 
{
	friend class TestStudyInPink;

    public:
        Map() {}
		Map(int num_rows, int num_cols, int num_walls, int num_fake_walls, Position *array_walls, Position *array_fake_walls)
		    : numRows(num_rows), numCols(num_cols)
		{ 
		    map = new MapElement**[num_rows]; 
			// Create all elements in the map PATH
		    for (int i = 0; i < num_rows; i++)
		    {
		        map[i] = new MapElement*[num_cols];
		        for (int j = 0; j < num_cols; j++) 
		            map[i][j] = new Path; 
		    }
		
			// Replace PATH at WALL positions
		    for (int i = 0; i < num_walls; i++)
		    {
		    	int rowWall = array_walls[i].getRow();
                int colWall = array_walls[i].getCol(); 
				delete map[rowWall][colWall]; // Xóa đối tượng cũ trước khi gán giá trị mới
				map[rowWall][colWall] = new Wall;
		    }

			// Replace PATH at FAKE_WALL positions
            for (int i = 0; i < num_fake_walls; i++)
		    {
		    	int rowFakeWall = array_fake_walls[i].getRow();
                int colFakeWall = array_fake_walls[i].getCol();
				delete map[rowFakeWall][colFakeWall]; // Xóa đối tượng cũ trước khi gán giá trị mới
				int fake_wall_exp = (rowFakeWall * 257 + colFakeWall * 139 + 89) % 900 + 1; // Calculate fake wall experience
        		map[rowFakeWall][colFakeWall] = new FakeWall(fake_wall_exp); 
		    }
		}
		~Map() 
		{
		    for (int i = 0; i < numRows; i++)
		        delete [] map[i];
		    delete [] map;
		}
		
		
		int getNumRows() const { return numRows; }
		int getNumCols() const { return numCols; }    
		MapElement*** getMap() const { return map; }    
		void setNumRows(int numRows) { this->numRows = numRows; }
		void setNumCols(int numCols) { this->numCols = numCols; }
		
		bool isValid(const Position& pos, MovingObject *mv_obj) const; // Check valid or invalid position
		
		void printElement()
		{
			for (int i = 0; i < numRows; i++)
			{
				for (int j = 0; j < numCols; j++)
				{
					ElementType type = map[i][j]->getType();
					switch (type)
					{
						case PATH : 
							cout << "P "; break;
						case WALL :
							cout << "W "; break;
						case FAKE_WALL:
							cout << "F "; break;
					}
				}
				cout << endl;
			}
		}

		void showFakeWallExperience() const // Show fake walls' positions
		{
			for (int i = 0; i < numRows; i++)
			{
				for (int j = 0; j < numCols; j++)
				{
					if (map[i][j]->getType() != FAKE_WALL) continue;
					FakeWall *fakewall = dynamic_cast<FakeWall*>(map[i][j]);
					cout << "\tFW(" << i << ", " << j << ") : " << fakewall->getReqExp() << endl;
				}
			}
		}

    private:
		int numRows; // Number of rows
        int numCols; // Number of columns
        MapElement ***map; // Pointer of all elements in the matrix 
};

class Character : public MovingObject // Create for character Sherlock, Watson and Criminal
{
	friend class TestStudyInPink;

	public :
		Character() : MovingObject() {}
		Character(int index, const Position& init_pos, Map *map, int init_hp, int init_exp) 
			: MovingObject(index, init_pos, map), hp(init_hp), exp(init_exp) 
		{
			hp = (init_hp <= 500) ? init_hp : 0;
		    exp = (init_exp <= 900) ? init_exp : 0;
		}

		int getHP() const { return hp; }
		int getEXP() const {return exp; }
		void setHP(int hp) { this->hp = hp; }
		void setEXP(int exp) { this->exp = exp; } 

	protected :
		int hp; // HP of character at current time
		int exp; // Experiences of character of current time
};

class SherLock : public Character // Sherlock Holmes
{
	friend class TestStudyInPink;

    public:
		SherLock() : Character() {}
        SherLock(int index, const string& moving_rule, const Position& init_pos, Map *map, int init_hp, int init_exp)
		    : Character(index, init_pos, map, init_hp, init_exp), moving_rule(moving_rule)
		{
		    nextMove = 0;
		    setName("Sherlock");
		}
		~SherLock() {}
		        
		Position getNextPosition() override
		{
		    if (nextMove == moving_rule.length()) nextMove = 0;
		
		    Position newPos;
		    if (moving_rule[nextMove] == 'L') 
		        newPos = Position(pos.getRow(), pos.getCol() - 1);
		    if (moving_rule[nextMove] == 'R')
		        newPos = Position(pos.getRow(), pos.getCol() + 1);
		    if (moving_rule[nextMove] == 'U')
		        newPos = Position(pos.getRow() - 1, pos.getCol());
		    if (moving_rule[nextMove] == 'D')
		        newPos = Position(pos.getRow() + 1, pos.getCol());
		
		    if (map->isValid(newPos, this))
		        return newPos;
		    else 
		        return Position::npos;
			return newPos;
		}
		Position getCurrentPosition() const override
		{
		    return getPosition();
		}
		void move() override
		{
		    Position nextPosition = getNextPosition();
		    nextMove++;
		    if (nextPosition == Position::npos) return;
		    pos = nextPosition;
		}
		string str() const override
		{
		    stringstream ss;
		    ss << "SherLock[index = " << index << "; ";
		    ss << "pos = " << pos.str() << "; ";
		    ss << "moving_rule = " << moving_rule << "]";
		    return ss.str();
		}
    
    private :
        string moving_rule;
        int nextMove; // current index of moving rule
};

class Watson : public Character // John H. Watson
{
	friend class TestStudyInPink;

    public:
        Watson( int index, 
				        const string& moving_rule, 
				        const Position& init_pos,
				        Map *map, 
				        int init_hp, 
				        int init_exp)
		    : Character(index, init_pos, map, init_hp, init_exp), moving_rule(moving_rule)
		{
		    nextMove = 0;
		    setName("Watson");
		}
		        
		Position getNextPosition() override
		{
		    if (nextMove == moving_rule.length()) nextMove = 0;
		
		    Position newPos;
		    if (moving_rule[nextMove] == 'L') 
		        newPos = Position(pos.getRow(), pos.getCol() - 1);
		    if (moving_rule[nextMove] == 'R')
		        newPos = Position(pos.getRow(), pos.getCol() + 1);
		    if (moving_rule[nextMove] == 'U')
		        newPos = Position(pos.getRow() - 1, pos.getCol());
		    if (moving_rule[nextMove] == 'D')
		        newPos = Position(pos.getRow() + 1, pos.getCol());
		
		    if (map->isValid(newPos, this))
		       return newPos;
		    else 
		        return Position::npos;
		}
		
		Position getCurrentPosition() const override { return getPosition(); }
		void move() override
		{
		    Position nextPosition = getNextPosition();
		    nextMove++;
		    if (nextPosition == Position::npos) return;
		    pos = nextPosition;
		}
		string str() const override
		{
		    stringstream ss;
		    ss << "Watson[index = " << index << "; ";
		    ss << "pos = " << pos.str() << "; ";
		    ss << "moving_rule = " << moving_rule << "]";
		    return ss.str();
		}

		int getHP() const { return hp; }
		int getEXP() const { return exp; }
        string getMovingRule() const { return moving_rule; }

		void setHP(int hp) { this->hp = hp; }
		void getEXP(int exp) { this->exp = exp; }
        void getMovingRule(string moving_rule) { this->moving_rule = moving_rule; }
    
    private :
        string moving_rule;
        int nextMove;
};

class Criminal : public Character // Criminal
{
	friend class TestStudyInPink;

    public :
        Criminal(int index, const Position& init_pos, Map *map, SherLock *sherlock, Watson *watson) 
			: Character(index, init_pos, map, 0, 0), sherlock(sherlock), watson(watson)
		{
			setName("Criminal");
		}
		
        ~Criminal() {};

        Position getNextPosition() override
		{
			int maxDistance = INT_MIN;
			Position resultPos = Position::npos;
			int dis[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

			for (int i = 0; i < 4; i++)
			{
				// Create position for each direction and find position that has max sum Mahattan
				Position next = Position(pos.getRow() + dis[i][0], pos.getCol() + dis[i][1]);
				int mahattanSherlock = Mahattan(sherlock->getCurrentPosition(), next);
				int mahattanWatson = Mahattan(watson->getCurrentPosition(), next);
				if (this->map->isValid(next, this) and maxDistance < mahattanSherlock + mahattanWatson)
				{
					resultPos = next;
					maxDistance = mahattanSherlock + mahattanWatson;
				}
			}

			return resultPos;
		}
		Position getCurrentPosition() const override { return getPosition(); }
		void move() override
		{
		    Position nextPosition = getNextPosition();
		
		    if (nextPosition == Position::npos) return;
		    pos = nextPosition;
		}
		string str() const override
		{
		    stringstream ss;
		    ss << "Criminal[index = " << index << "; ";
		    ss << "pos =" << pos.str() << "]";
		    return ss.str();
		}
		
		private :
		    SherLock *sherlock; // pointer of sherlock
		    Watson *watson; // pointer of watson
};

class BaseItem
{
	friend class TestStudyInPink;

	public :
		BaseItem() {}
		~BaseItem() {}

		virtual bool canUse(Character *obj, Robot *robot) = 0;
		virtual void use(Character *obj, Robot *robot) = 0;

		ItemType getType() const
		{
			return type;
		}
		void setType(ItemType type)
		{
			this->type = type;
		}

		virtual string str() const = 0;

	protected :
		ItemType type;
};

class MagicBook : public BaseItem // Sách ảo ma
{
	friend class TestStudyInPink;

	public :
		MagicBook() { type = MAGIC_BOOK; }
		~MagicBook() {}

		bool canUse(Character *obj, Robot *robot) override
		{
			// Nhớ thêm tham số robot, nếu ko có robot thì cho là NULL
			if (robot != NULL) return false; // gặp robot thì ko dùng đc
			return (obj->getEXP() <= 350);
		}

		void use(Character *obj, Robot *robot) override
		{
			if (canUse(obj, robot))
				obj->setEXP(int(obj->getEXP() * 1.25)); // Hồi phục 25% máu
		}

		string str() const override
		{
			return "MAGIC BOOK";
		}
};

class EnergyDrink : public BaseItem // Nước tăng lực
{
	friend class TestStudyInPink;

	public :
		EnergyDrink() { type = ENERGY_DRINK; }
		~EnergyDrink() {}

		bool canUse(Character *obj, Robot *robot) override
		{
			// Robot hoặc NULL
			if (robot != NULL) return false; // gặp robot thì không dùng được
			return (obj->getHP() <= 100);
		}

		void use(Character *obj, Robot *robot) override
		{
			if (canUse(obj, robot))
				obj->setEXP(int(obj->getHP() * 1.2)); // Hồi 205 exp
		}

		string str() const override
		{
			return "ENERGY DRINK";
		}
};

class FirstAid : public BaseItem // Hộp cứu thương
{
	friend class TestStudyInPink;

	public :
		FirstAid() { type = FIRST_AID; }
		~FirstAid() {}

		bool canUse(Character *obj, Robot *robot) override
		{
			// Robot hoặc NULL
			if (robot != NULL) return false; // Nếu gặp robot thì ko dùng đc
			return (obj->getHP() <= 100) or (obj->getEXP() <= 350);
		}

		void use(Character *obj, Robot *robot) override
		{
			if (canUse(obj, robot))
			{
				if (obj->getHP() * 1.5 > 900) obj->setHP(900);
				else obj->setHP(obj->getHP() * 1.5); //hồi 50% máu
			}
		}

		string str() const override
		{
			return "FIRST AID";
		}
};

class ExcemptionCard : public BaseItem // Thẻ miễn trừ
{
	friend class TestStudyInPink;

	public :
		ExcemptionCard() { type = EXCEMPTION_CARD; }
		~ExcemptionCard() {}

		bool canUse(Character *obj, Robot *robot) override
		{
			// robot hoặc NULL
			if (robot == NULL) return false; // nếu ko có robot thì ko dùng đc

			// có robot thì xem xét
			return (obj->getName() == "Sherlock") and (obj->getHP() % 2 == 1);
		}

		void use(Character *obj, Robot *robot) override
		{
			
		}

		string str() const override
		{
			return "EXCEMPTION CARD";
		}
};

class PassingCard : public BaseItem // Thẻ bỏ qua
{
	friend class TestStudyInPink;

	public :
		PassingCard() { type = PASSING_CARD; }
		PassingCard(const string& challenge) : challenge(challenge) { type = PASSING_CARD; }

		bool canUse(Character *obj, Robot *robot) override
		{
			// Robot hoặc NULL

			if (robot == NULL) return false; // chỉ dùng đc khi gặp robot
			return (obj->getName() == "Watson") and (obj->getHP() % 2 == 0);
		}

		void use(Character *obj, Robot *robot) override
		{
			
		}	
		~PassingCard() {}

		string getChallenge() const { return challenge; }
		void setChallennge(string challenge) { this->challenge = challenge; }	

		string str() const override
		{
			return "PASSING CARD - " + challenge;	
		}

	private :
		string challenge;
};

/*
Sau mỗi 3 bước đi của tên tội phạm một robot sẽ được tạo ra tại vị trí trước đó mà
tên tội phạm đang đứng. Các loại robot và điều kiện tạo ra tương ứng:
• Nếu là robot đầu tiên được tạo ra trên bản đồ, đó sẽ là loại robot RobotC. Nếu không,
ta xét khoảng cách từ Robot đến Sherlock và Watson:
• Nếu khoảng cách đến Sherlock gần hơn: Tạo ra loại robot RobotS
• Khoảng cách đến Watson gần hơn: Tạo ra loại robot RobotW
• Khoảng cách đến Sherlock và Watson là bằng nhau: Tạo ra loại robot RobotSW
*/

class Robot : public MovingObject // Class for all robot types
{
	friend class TestStudyInPink;

	public :
		Robot(int index, const Position& init_pos, Map *map) : MovingObject(index, init_pos, map)
		{
			// Tinh so chu dao
			int p = init_pos.getRow() * init_pos.getCol();
			while (p >= 10)
			{
				int sum = 0;
				while (p > 0)
				{
					sum += p % 10;
					p /= 10;
				}
				p = sum;
			}
			// Tao vat pham co trong robot
			if (p >= 8) 
			{
				// tao thuoc tinh challenge
				int t = (init_pos.getRow() * 11 + init_pos.getCol()) % 4;
				if (t == 0) item = new PassingCard("RobotS");
				else if (t == 1) item = new PassingCard("RobotC");
				else if (t == 2) item = new PassingCard("RobotSW");
				else item = new PassingCard("all");
			}
			else if (p >= 6) item = new ExcemptionCard;
			else if (p >= 4) item = new FirstAid;
			else if (p >= 2) item = new EnergyDrink;
			else item = new MagicBook;
		}

		string str() const
		{
			stringstream ss;
			ss << "Robot[pos = " << pos.str() << "; ";
			ss << "type = " << robot_type << "; ";
			ss << "dist = ";
			if (robot_type == C) ss << "\"\"]"; // Robot C have no dist
			else ss << getDistance() << "]";

			return ss.str();
		}

		void move()
		{
			Position next = getNextPosition();
			if (next != Position::npos)
				this->pos = next;
		}

		virtual int getDistance() const = 0;

		string getItemName() const
		{
			return item->str();
		}

		BaseItem* getItemInside() const { return item; }

	protected :
		RobotType robot_type; // Kieu robot
		BaseItem *item; // Vật phẩm nằm trong robot
};

class RobotC : public Robot // RobotC
{
	friend class TestStudyInPink;

	public :
		RobotC(int index, const Position& init_pos, Map *map , Criminal *criminal)
			: Robot(index, init_pos, map), criminal(criminal)
		{
			this->name = "RobotC";
			this->robot_type = C;
		}
		~RobotC() {}

		Position getNextPosition() override
		{
			if (this->map->isValid(criminal->getCurrentPosition(), this)) return criminal->getCurrentPosition();
			return Position::npos;
		}

        Position getCurrentPosition() const override { return pos; }

		int getDistance() const override
		{
			return 1;
		}

		int getDistance(Character *Object) const
		{
			// Object can be Sherlock or watson
			Position obj_pos = Object->getPosition();
			return Mahattan(this->pos, obj_pos);
		}

	private :
		Criminal *criminal; // pointer of criminal
};

class RobotS : public Robot // RobotS
{
	friend class TestStudyInPink;

	public :
		RobotS(int index, const Position& init_pos, Map *map , Criminal *criminal, SherLock *sherlock)
			: Robot(index, init_pos, map), criminal(criminal), sherlock(sherlock)
		{
			this->name = "RobotS";
			this->robot_type = S;
		}

		Position getNextPosition() override
		{
			// Find first valid position
			for (int i = 0; i < 4; i++)
			{
				Position next = Position(this->pos.getRow() + dis[i][0], this->pos.getCol() + dis[i][1]);
				if (this->map->isValid(next, this) and getDistance() > Mahattan(sherlock->getCurrentPosition(), next))
					return next;
			}
			
			return Position::npos;
		}

        Position getCurrentPosition() const override { return pos; }

		int getDistance() const override
		{
			Position obj_pos = sherlock->getCurrentPosition();
			return Mahattan(this->pos, obj_pos);
		}

	private :
		Criminal *criminal;
		SherLock *sherlock;
};

class RobotW : public Robot // Robot Watson
{
	friend class TestStudyInPink;

	public :
		RobotW (int index, const Position& init_pos, Map *map , Criminal *criminal, Watson *watson)
			: Robot(index, init_pos, map), criminal(criminal), watson(watson)
		{
			this->name = "RobotW";
			this->robot_type = W;
		}

		Position getNextPosition() override
		{
			// Find first valid position
			for (int i = 0; i < 4; i++)
			{
				Position next = Position(this->pos.getRow() + dis[i][0], this->pos.getCol() + dis[i][1]);
				if (this->map->isValid(next, this) and getDistance() > Mahattan(watson->getCurrentPosition(), next))
					return next;
			}
			
			return Position::npos;
		}

        Position getCurrentPosition() const override { return pos; }

        void move() override
		{
			Position next = getNextPosition();
			if (next != Position::npos)
				this->pos = next;
		}

		int getDistance() const override
		{
			Position obj_pos = watson->getCurrentPosition();
			return Mahattan(this->pos, obj_pos);
		}
	private :
		Criminal *criminal;
		Watson *watson;
};

class RobotSW : public Robot // Robot Sherlock watson
{ 
	friend class TestStudyInPink;

	public :
		RobotSW (int index, const Position& init_pos, Map *map , Criminal *criminal, SherLock *sherlock , Watson *watson)
			: Robot(index, init_pos, map), criminal(criminal), sherlock(sherlock), watson(watson)
		{
			this->name = "RobotSW";
			this->robot_type = SW;
		}

		Position getNextPosition() override
		{
			// Find first valid position
			for (int i = 0; i < 4; i++)
			{
				Position next = Position(this->pos.getRow() + dis[i][0], this->pos.getCol() + dis[i][1]);
				if (this->map->isValid(next, this) and getDistance() > Mahattan(sherlock->getCurrentPosition(), next) + Mahattan(watson->getCurrentPosition(), next))
					return next;
			}
			
			return Position::npos;
		}

        Position getCurrentPosition() const override { return pos; }

		int getDistance() const override
		{
			Position sherlock_dis = sherlock->getCurrentPosition();
			Position watson_dis = watson->getCurrentPosition();
			return Mahattan(this->pos, sherlock_dis) + Mahattan(this->pos, watson_dis);
		}

	private :
		Criminal *criminal;
		SherLock *sherlock;
		Watson *watson;
};

// isValid for all characters....
bool Map::isValid(const Position& pos, MovingObject *mv_obj) const
{  	
	bool check_row, check_col;
	if ((pos.getRow() < 0) or (pos.getRow() >= numRows)) check_row = true; else check_row = false;
	if ((pos.getCol() < 0) or (pos.getCol() >= numCols)) check_col = true; else check_col = false;
	if (check_row or check_col) return false;

	
	
	bool checkWall, checkFakeWall;
	ElementType check = map[pos.getRow()][pos.getCol()]->getType();
	if (check == WALL) checkWall = true; else checkWall = false;
	if (check == FAKE_WALL) checkFakeWall = true; else checkFakeWall = false;
	if (checkWall) return false;

	if (mv_obj->getName() == "Watson" and checkFakeWall)
	{
		Watson* watson = dynamic_cast<Watson*>(mv_obj);
		FakeWall* fakeWall = dynamic_cast<FakeWall*>(map[pos.getRow()][pos.getCol()]);
		return (watson->getEXP() > fakeWall->getReqExp());
	}

	return true;
}

class BaseBag
{
	friend class TestStudyInPink;

	public :
		BaseBag(Character *obj_in) : obj(obj_in) 
		{ 
			count = 0;
		}

		~BaseBag() 
		{
			delete [] item;
		}

		virtual bool insert(BaseItem *Item) = 0; // return true if insert successfully
		virtual BaseItem* get() = 0; // return the item as described above , if not found , return NULL
		virtual BaseItem* get(ItemType itemType) = 0; // return the item as described above , if not found , return NULL
		virtual string str() const = 0;

		int getCapacity() const { return capacity; }
		int getCount() const { return count; }
		Character* getObj() const { return obj; }

	protected :
		int capacity;
		int count;
		Character *obj;
		BaseItem **item;
};

class SherlockBag : public BaseBag
{
	friend class TestStudyInPink;

	public :
		SherlockBag(SherLock *sherlock) : BaseBag(sherlock) 
		{
			capacity = 13;
			item = new BaseItem*[capacity];
			for (int i = 0; i < capacity; i++) item[i] = nullptr; 
		}
		~SherlockBag() {}

		bool insert(BaseItem *Item) override
		{
			if (count == capacity) return false;
			item[count++] = Item;
			return true;
		}

		BaseItem* get() override
		{
			int i = count - 1;
			while (i >= 0)
			{
				if (item[i]->canUse(obj, NULL)) break; 
				i--;
			}

			if (i < 0) return NULL; 
			else if (i == count - 1)
			{
				count--;
				return item[i];
			}
			else 
			{
				BaseItem *temp = item[i];
				item[i] = item[count - 1];
				count--;
				return temp;
			}
		}

		BaseItem* get(ItemType type) override
		{
			int i = count - 1;
			while (i >= 0 and item[i]->getType() != type) cout << item[i--]->str() << ' ';
			
			if (i < 0) return NULL; 
			else if (i == count - 1)
			{
				count--;
				return item[i];
			}
			else
			{
				BaseItem *temp = item[i];
				item[i] = item[count - 1];
				count--;
				return temp;
			}

			return NULL;
		}

		string str() const override
		{
			stringstream ss;
			ss << "Bag[count = " << getCount() << "; ";
			for (int i = 0; i < count; i++)
			{
				ss << item[i]->str();
				if (i != count - 1) ss << ", ";
			}
			ss << "]";

			return ss.str();
		}
};

class WatsonBag : public BaseBag
{
	friend class TestStudyInPink;

	public :
		WatsonBag(Watson *watson) : BaseBag(watson) 
		{
			capacity = 15;
			item = new BaseItem*[capacity];
			for (int i = 0; i < capacity; i++) item[i] = nullptr; 
		}
		~WatsonBag() {}

		bool insert(BaseItem *Item) override
		{
			if (count == capacity) return false;
			item[count++] = Item;
			return true;
		}

		BaseItem* get() override
		{
			int i = count - 1;
			while (i >= 0)
			{
				if (item[i]->canUse(obj, NULL)) break;
				i--;
			}
			if (i < 0) return NULL; 
			else if (i == count - 1)
			{
				count--;
				return item[i];
			}
			else 
			{
				BaseItem *temp = item[i];
				item[i] = item[count - 1];
				count--;
				return temp;
			}
		}

		BaseItem* get(ItemType type) override
		{
			int i = count - 1;
			while (i >= 0 and item[i]->getType() != type) i--;

			if (i < 0) return NULL; 
			else if (i == count - 1)
			{
				count--;
				return item[i];
			}
			else
			{
				BaseItem *temp = item[i];
				item[i] = item[count - 1];
				count--;
				return temp;
			}

			return NULL;
		}

		string str() const override
		{
			stringstream ss;
			ss << "Bag[count = " << getCount() << "; ";
			for (int i = 0; i < count; i++)
			{
				ss << item[i]->str();
				if (i != count - 1) ss << ", ";
			}
			ss << "]";

			return ss.str();
		}
};

class ArrayMovingObject
{
	friend class TestStudyInPink;

	public :
		ArrayMovingObject(int capacity) : capacity(capacity), count(0) 
		{ arr_mo_objs = new MovingObject*[capacity]; } 
		~ArrayMovingObject() { delete [] arr_mo_objs; }

		bool isFull() const { return count == capacity; }
		bool add(MovingObject *mv_obj) 
		{ 
			if (isFull()) return false;
			arr_mo_objs[count++] = mv_obj;
			return true;
		}
		string str() const
		{
			stringstream ss;
			ss << "ArrayMovingObject[count ";
			ss << getCount() << "; ";
			ss << "capacity = " << getCapacity();
			for (int i = 0; i < count; i++)
			{ 
				ss << "; ";
				ss << arr_mo_objs[i]->str();
			}
			ss << "]";

			return ss.str();
		}
		
		int size() const {
			return count;
		}
		
		MovingObject* get(int index)
		{
			return arr_mo_objs[index];
		}
		
		MovingObject** getArrayMovingObject() const { return arr_mo_objs; }
		int getCount() const { return count; }
		int getCapacity() const { return capacity; }

		void setCapacity(int capacity) { this->capacity = capacity; }

	private :
		MovingObject **arr_mo_objs;
		int count;
		int capacity;
};

class Configuration
{
	friend class TestStudyInPink;
	friend class StudyInPinkProgram;

	public :
		Configuration(const string& filepath)
		{
			ifstream input;
			input.open(filepath);

			// Luu vao het vao 1 mang
			int i = 0;
			while (!input.eof())
			{
				string temp; getline(input, temp, '\n');
				file[i++] = temp;
			}

			string temp; 

			// Read map_num-rows
			for (int i = file[0].length() - 1; file[0][i] != '='; i--)
				temp = file[0][i] + temp;
			map_num_rows = stoi(temp);
			cout << "Numbers of rows : " << map_num_rows << endl;
			
			// Read map_num_cols
			temp = "";
			for (int i = file[1].length() - 1; file[1][i] != '='; i--)
				temp = file[1][i] + temp;
			map_num_cols = stoi(temp);
			cout << "Numbers of columns : " << map_num_cols << endl;
	
			// Read max_num_moving_objects
			temp = "";
			for (int i = file[2].length() - 1; file[2][i] != '='; i--)
				temp = file[2][i] + temp;
			max_num_moving_objects = stoi(temp);
			cout << "Max number of moving objects : " << max_num_moving_objects << endl;

			// Read arr_walls
			temp = ""; 
			num_walls = 1;
			for (int i = file[3].length() - 2; file[3][i] != '['; i--)
			{
				temp = file[3][i] + temp;
				if (file[3][i] == ';') num_walls++;
			}
			cout << "Number of walls : " << num_walls << endl;
			arr_walls = new Position[num_walls];
			int index = 0; i = 0; 
			while (i < temp.length())
			{
				string pos;
				while (temp[i] != ';')
					pos += temp[i++];
				arr_walls[index++] = Position(pos);
				i++;
			}
			cout << "Array of walls : ";
			for (int i = 0; i < num_walls; i++)
			{
				cout << "(" << arr_walls[i].getRow() << "," << arr_walls[i].getCol() << ")";
				if (i != num_walls - 1) cout << ", ";
			} 
			cout << endl;

			//Read arr_fake_walls
			temp = ""; 
			num_fake_walls = 1;
			for (int i = file[4].length() - 2; file[4][i] != '['; i--)
			{
				temp = file[4][i] + temp;
				if (file[4][i] == ';') num_fake_walls++;
			}
			cout << "Number of fake walls : " << num_fake_walls << endl;
			arr_fake_walls = new Position[num_fake_walls];
			index = 0; i = 0; 
			while (i < temp.length())
			{
				string pos;
				while (temp[i] != ';')
					pos += temp[i++];
				arr_fake_walls[index++] = Position(pos);
				i++;
			}
			cout << "Array of fake walls : ";
			for (int i = 0; i < num_fake_walls; i++)
			{
				cout << "(" << arr_fake_walls[i].getRow() << "," << arr_fake_walls[i].getCol() << ")";
				if (i != num_fake_walls - 1) cout << ", ";
			} 
			cout << endl;

			// Read sherlock_moving_rule
			temp = "";
			for (int i = file[5].length() - 1; file[5][i] != '='; i--)
				temp = file[5][i] + temp;
			sherlock_moving_rule = temp;
			cout << "Sherlock's moving rule : " << sherlock_moving_rule << endl;

			// Read sherlock_init_pos
			temp = "";
			for (int i = file[6].length() - 1; file[6][i] != '='; i--)
				temp = file[6][i] + temp;
			sherlock_init_pos = Position(temp);
			cout << "Sherlock's initial position : " << "(" << sherlock_init_pos.getRow() << "," << sherlock_init_pos.getCol() << ")" << endl; 

			// Read sherlock_init_hp
			temp = "";
			for (int i = file[7].length() - 1; file[7][i] != '='; i--)
				temp = file[7][i] + temp;
			sherlock_init_hp = stoi(temp);
			cout << "Sherlock's initial HP : " << sherlock_init_hp << endl;

			// Read sherlock_init_exp
			temp = "";
			for (int i = file[8].length() - 1; file[8][i] != '='; i--)
				temp = file[8][i] + temp;
			sherlock_init_exp = stoi(temp);
			cout << "Sherlock's initial EXP : " << sherlock_init_exp << endl;

			// Read watson_moving_rule
			temp = "";
			for (int i = file[9].length() - 1; file[9][i] != '='; i--)
				temp = file[9][i] + temp;
			watson_moving_rule = temp;
			cout << "Watson's moving rule : " << watson_moving_rule << endl;
			// Read watson_init_pos
			temp = "";
			for (int i = file[10].length() - 1; file[10][i] != '='; i--)
				temp = file[10][i] + temp;
			watson_init_pos = Position(temp);
			cout << "Watson's initial position : " << "(" << watson_init_pos.getRow() << "," << watson_init_pos.getCol() << ")" << endl;

			// Read watson_init_hp
			temp = "";
			for (int i = file[11].length() - 1; file[11][i] != '='; i--)
				temp = file[11][i] + temp;
			watson_init_hp = stoi(temp);
			cout << "Watson's initial HP : " << watson_init_hp << endl;

			// Read watson_init_exp
			temp = "";
			for (int i = file[12].length() - 1; file[12][i] != '='; i--)
				temp = file[12][i] + temp;
			watson_init_exp = stoi(temp);
			cout << "Watson's initial EXP : " << watson_init_exp << endl;

			// Read Criminal_init_pos
			temp = "";
			for (int i = file[13].length() - 1; file[13][i] != '='; i--)
				temp = file[13][i] + temp;
			criminal_init_pos = Position(temp);
			cout << "Criminal's initial position : " << "(" << criminal_init_pos.getRow() << "," << criminal_init_pos.getCol() << ")" << endl;

			// Read num_steps;
			temp = "";
			for (int i = file[14].length() - 1; file[14][i] != '='; i--)
				temp = file[14][i] + temp;
			num_steps = stoi(temp);
			cout << "Number of steps : " << num_steps << endl;
		}
		~Configuration()
		{
			delete [] arr_walls;
			delete [] arr_fake_walls;
		}


		int get_map_num_rows() const { return map_num_rows; }
		int get_map_num_cols() const { return map_num_cols; }
		int get_max_num_moving_objects() const { return max_num_moving_objects; }
		int get_num_walls() const { return num_walls; }
		Position *get_arr_walls() const { return arr_walls; }
		int get_num_fake_walls() const { return num_fake_walls; }
		Position *get_arr_fake_walls() const { return arr_fake_walls; }
		string get_sherlock_moving_rule() const { return sherlock_moving_rule; }
		Position get_sherlock_init_pos() const { return sherlock_init_pos; }
		int get_sherlock_init_hp() const { return sherlock_init_hp; }
		int get_sherlock_init_exp() const { return sherlock_init_exp; }
		string get_watson_moving_rule() const { return watson_moving_rule; }
		Position get_watson_init_pos() const { return watson_init_pos; }
		int get_watson_init_hp() const { return watson_init_hp; }
		int get_watson_init_exp() const { return watson_init_exp; }
		Position get_criminal_init_pos() const { return criminal_init_pos; }
		int get_num_steps() const { return num_steps; }

		string str() const
		{
			stringstream ss;
			ss << "Configuration[\n";
			for (int i = 0; i < 15; i++)
			{
				ss << file[i] << '\n';
			}
			ss << "]";

			return ss.str();
		}

	private :
		// rows and cols of map
		int map_num_rows;
		int map_num_cols;

		// Max number of moving objects
		int max_num_moving_objects;

		// Walls
		int num_walls;
		Position *arr_walls;

		//FakeWalls
		int num_fake_walls;
		Position *arr_fake_walls;

		// Sherlock
		string sherlock_moving_rule;
		Position sherlock_init_pos;
		int sherlock_init_hp;
		int sherlock_init_exp;

		// Watson
		string watson_moving_rule;
		Position watson_init_pos;
		int watson_init_hp;
		int watson_init_exp;

		//Criminal
		Position criminal_init_pos;

		//Number of steps
		int num_steps;

		//file read
		string file[15];
};

class StudyInPinkProgram
{
	friend class TestStudyInPink;

	public :
		StudyInPinkProgram(const string& config_file_path)
		{
			config = new Configuration(config_file_path);
			arr_mov_objs = new ArrayMovingObject(10);
			arr_mov_objs->add(criminal);
			arr_mov_objs->add(sherlock);
			arr_mov_objs->add(watson);
		}
		~StudyInPinkProgram();

		bool isStop() const
		{
			if (!sherlock->getHP() or !watson->getHP()) return true;
			if (sherlock->getCurrentPosition() == criminal->getCurrentPosition()) return true;
			if (watson->getCurrentPosition() == criminal->getCurrentPosition()) return true;
			return false;
		}

		void printResult() const 
		{
			if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
				cout << "Sherlock caught the criminal" << endl;
			}
			else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
				cout << "Watson caught the criminal" << endl;
			}
			else {
				cout << "The criminal escaped" << endl;
			}
		}

		void printStep(int si) const {
			cout << "Step: " << setw(4) << setfill('0') << si
				<< "--"
				<< sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
		}

		void run(bool verbose) {
			// Note: This is a sample code. You can change the implementation as you like.
			// TODO
			for (int istep = 0; istep < config->num_steps; ++istep) {
				for (int i = 0; i < arr_mov_objs->size(); ++i) {
					arr_mov_objs->get(i)->move();
					if (isStop()) {
						printStep(istep);
						break;
					}
					if (verbose) {
						printStep(istep);
					}
				}
			}
			printResult();
		}

    
	private :
		Configuration *config;
		SherLock *sherlock;
		Watson *watson;
		Criminal *criminal;
		Map *map;
		ArrayMovingObject *arr_mov_objs;

};

class TestStudyInPink {
	public:
		TestStudyInPink() = default;

		static void satc_01() {
			cout << "----- Sample Testcase 01 -----" << endl;
			int num_walls = 3;
			Position arr_walls[] = {Position(1, 2), Position(2, 3), Position(3, 4)};
			int num_fake_walls = 1;
			Position arr_fake_walls[] = {Position(2, 0)};

			Map *map = new Map(10, 10, num_walls, num_fake_walls, arr_walls, arr_fake_walls);
			
			SherLock *sherlock = new SherLock(1, "RUU", Position(1, 3), map, 250, 450);
			cout << sherlock->str() << endl;

			Watson *watson = new Watson(2, "LU", Position(2, 1), map, 300, 350);
			cout << watson->str() << endl;

			Criminal *criminal = new Criminal(0, Position(7, 9), map, sherlock, watson);
			cout << criminal->str() << endl;

			cout << "* Sherlock makes a move" << endl;
			sherlock->move();
			cout << sherlock->str() << endl;

			cout << "* Watson makes a move" << endl;
			watson->move();
			cout << watson->str() << endl;

			cout << "* Criminal makes a move" << endl;
			criminal->move();
			cout << criminal->str() << endl;

			ArrayMovingObject * arr_mv_objs = new ArrayMovingObject(10);
			arr_mv_objs->add(criminal);
			arr_mv_objs->add(sherlock);
			arr_mv_objs->add(watson);
			cout << arr_mv_objs->str() << endl;
			
			delete arr_mv_objs;
			delete sherlock;
			delete watson;
			delete criminal;
			delete map;
		}

		static void satc_02() {
			// g_satc_02();
		}

		static void satc_03() {
			// Sample test for RobotC
			cout << "----- Sample Testcase 03 -----" << endl;
			int num_walls = 3;
			Position arr_walls[] = {Position(1, 2), Position(2, 3), Position(3, 4)};
			int num_fake_walls = 1;
			Position arr_fake_walls[] = {Position(2, 0)};

			Map * map = new Map(10, 10, num_walls, num_fake_walls, arr_walls, arr_fake_walls);

			SherLock * sherlock = new SherLock(1, "RUU", Position(1, 3), map, 250, 450);
			Watson * watson = new Watson(2, "LU", Position(2, 1), map, 300, 350);
			Criminal * criminal = new Criminal(0, Position(7, 9), map, sherlock, watson);
			criminal->move();

			RobotC * robotC = new RobotC(3, Position(7,9), map, criminal);
			cout << "Current position of Criminal: " << criminal->str() << endl;
			cout << "Criminal makes a move" << endl;
			criminal->move();
			cout << "New position of Criminal: " << criminal->str() << endl;

			cout << "Current position of robotC: " << robotC->str() << endl;
			cout << "RobotC makes a move" << endl;
			robotC->move();
			cout << "New position of robotC: " << robotC->str() << endl;

			delete sherlock;
			delete watson;
			delete criminal;
			delete map;
			delete robotC;
		}
};

/////////////////////// Genetic Functions
int Mahattan(const Position& a, const Position& b)
{
	return abs(a.getRow() - b.getRow()) + abs(a.getCol() - b.getCol());
}

main(int argc, const char * argv[])
{
	// Create Map.....
	const int row = 10, col = 10; // row and column
	const int num_walls = 9; // numbers of wall
	Position array_wall[num_walls] = 
		{ 
			Position(1, 1), Position(2, 1), Position(3, 1), Position(3, 2),
			Position(6, 2), Position(6, 3), Position(6, 4), Position(7, 4),
			Position(6, 9)
		}; // array of walls
	const int num_fake_walls = 8;
	Position array_fake_walls[num_fake_walls] =
		{
			Position(0, 4), Position(1, 4), Position(2, 4),
			Position(1, 8),
			Position(4, 7),
			Position(8, 6), Position(9, 6), Position(9, 7),
		}; // array of fake walls
	Map map(row, col, num_walls, num_fake_walls, array_wall, array_fake_walls); // Map pointer
	map.printElement(); cout << endl; // Show map


	// Show Fakewall experience
	cout << "Show fake wall experience : " << endl;
	map.showFakeWallExperience(); cout << endl;


	// Making charater Sherlock Holmes.....
	const int indexSherlock = 0; // Index in array moving object of Holmes
	const string movingRuleSherLock = "UR"; // Holmes's moving rule
	const Position init_pos_Sherlock = Position(8, 1); // Initial Holmes's position
	const int init_hp_sherlock = 400; // Initial Holmes's HP
	const int init_exp_sherlock = 700; // Initial Holmes's experience
	SherLock sherlock(indexSherlock, movingRuleSherLock, init_pos_Sherlock, &map, init_hp_sherlock, init_exp_sherlock);
	cout << sherlock.str() << endl; // Show Holmes's informations


	// Making charater John Watson.....
	const int indexWatson = 1; // Index in array moving object of Watson
	const string movingRuleWatson = "LU"; // Watson's moving rule
	const Position init_pos_Watson = Position(7, 8); // Initial Watson's position
	const int init_hp_watson = 400; // Initial Watson's HP
	const int init_exp_watson = 300; // Initial Watson's experience
	Watson watson(indexWatson, movingRuleWatson, init_pos_Watson, &map, init_hp_watson, init_exp_watson);
	cout << watson.str() << endl; // Show Watson's informations


	// Making character Criminal.....
	const int indexCriminal = 2; // Index in array moving object of Criminal
	const Position init_pos_Criminal = Position(3, 5);
	Criminal criminal(indexCriminal, init_pos_Criminal, &map, &sherlock, &watson);
	cout << criminal.str() << endl << endl; // Show criminal's informations
	
	
	// Test isValid for Sherlock Holmes
	cout << "Test isValid for Sherlock Holmes" << endl;
	cout << "\tPosition (4, 3) : "; if (map.isValid(Position(4, 3), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (5, 9) : "; if (map.isValid(Position(5, 9), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (6, 3) : "; if (map.isValid(Position(6, 3), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (2, 4) : "; if (map.isValid(Position(2, 4), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (13, 14) : "; if (map.isValid(Position(13, 14), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (2, 20) : "; if (map.isValid(Position(2, 20), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (14, 7) : "; if (map.isValid(Position(14, 7), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << endl;

	// Test isValid for Watson
	cout << "Test isValid for Watson" << endl;
	cout << "\tPosition (9, 9) : "; if (map.isValid(Position(9, 9), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (1, 4) : "; if (map.isValid(Position(1, 4), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (7, 4) : "; if (map.isValid(Position(7, 4), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (9, 7) : "; if (map.isValid(Position(9, 7), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (11, 7) : "; if (map.isValid(Position(11, 7), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (1, 13) : "; if (map.isValid(Position(1, 13), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (10, 11) : "; if (map.isValid(Position(10, 11), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << endl;

	// Test isValid for criminal
	cout << "Test isValid for criminal" << endl;
	cout << "\tPosition (10, 5) : "; if (map.isValid(Position(10, 5), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (6, 9) : "; if (map.isValid(Position(6, 9), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (2, 16) : "; if (map.isValid(Position(2, 16), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (8, 6) : "; if (map.isValid(Position(8, 6), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (10, 16) : "; if (map.isValid(Position(10, 16), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << endl;
	

	// Test robotC
	int RobotC_index = 3;
	Position RobotC_init_position(4, 5);
	RobotC robotc(RobotC_index, RobotC_init_position, &map, &criminal);
	cout << "Distance from RobotC to Sherlock : " << robotc.getDistance(&sherlock) << endl;
	cout << "Distance from RobotC to Watson : " << robotc.getDistance(&watson) << endl;
	cout << robotc.str() << endl;
	cout << "Item inside : " << robotc.getItemName() << endl;
	cout << "RobotC moved" << endl;
	robotc.move(); 
	cout << robotc.str() << endl;
	cout << "RobotC moved" << endl;
	robotc.move(); 
	cout << robotc.str() << endl;
	cout << endl;
	

	// Test robotS
	int RobotS_index = 4;
	Position RobotS_init_position(6, 6);
	RobotS robots(RobotS_index, RobotS_init_position, &map, &criminal, &sherlock);
	cout << "Distance from RobotS to Sherlock : " << robots.getDistance() << endl;
	cout << robots.str() << endl;
	cout << "Item inside : " << robots.getItemName() << endl;
	cout << "RobotS moved" << endl;
	robots.move(); 
	cout << robots.str() << endl;
	cout << "RobotS moved" << endl;
	robots.move(); 
	cout << robots.str() << endl;
	cout << endl;

	// Test robotW
	int RobotW_index = 5;
	Position RobotW_init_position(3, 9);
	RobotW robotw(RobotW_index, RobotW_init_position, &map, &criminal, &watson);
	cout << "Distance from RobotW to Watson : " << robotw.getDistance() << endl;
	cout << robotw.str() << endl;
	cout << "Item inside : " << robotw.getItemName() << endl;
	cout << "RobotW moved" << endl;
	robotw.move(); 
	cout << robotw.str() << endl;
	cout << "RobotW moved" << endl;
	robotw.move(); 
	cout << robotw.str() << endl;
	cout << endl;

	
	// Test robotSW
	int RobotSW_index = 6;
	Position RobotSW_init_position(9, 4);
	RobotSW robotsw(RobotSW_index, RobotSW_init_position, &map, &criminal, &sherlock, &watson);
	cout << "Total distance from RobotS to Sherlock and to Watson : " << robotsw.getDistance() << endl;
	cout << robotsw.str() << endl;
	cout << "Item inside : " << robotsw.getItemName() << endl;
	cout << "RobotSW moved" << endl;
	robotsw.move(); 
	cout << robotsw.str() << endl;
	cout << "RobotSW moved" << endl;
	robotsw.move(); 
	cout << robotsw.str() << endl;
	cout << endl;


	// Make array moving objects
	ArrayMovingObject arr_mov_objs(7);
	if(arr_mov_objs.add(&sherlock)) cout << "Added Sherlock" << endl;
	if(arr_mov_objs.add(&watson)) cout << "Added Watson" << endl;
	if(arr_mov_objs.add(&criminal)); cout << "Added criminal" << endl;
	if(arr_mov_objs.add(&robotc)) cout << "Added RobotC" << endl;
	if(arr_mov_objs.add(&robots)) cout << "Added RobotS" << endl;
	if(arr_mov_objs.add(&robotw)) cout << "Added RobotW" << endl;
	if(arr_mov_objs.add(&robotsw)) cout << "Added RobotSW" << endl;
	cout << arr_mov_objs.str() << endl;
	cout << endl;


	// Test Sherlock, Watson, criminal and robot move
	for (int i = 0; i < 10; i++)
	{
		arr_mov_objs.getArrayMovingObject()[i % 3]->move(); 
		cout << arr_mov_objs.getArrayMovingObject()[i % 3]->str() << endl;
	}
	cout << endl;
	
	// Test sherlockBag
	SherlockBag sherlockBag(&sherlock);
	sherlockBag.insert(robotc.getItemInside());
	cout << "Sherlock get an item from robotC: " << robotc.getItemInside()->str() << endl;
	BaseItem *item1 = new EnergyDrink;
	BaseItem *item2 = new ExcemptionCard;
	BaseItem *item3 = new MagicBook;
	BaseItem *item4 = new PassingCard("RobotC");
	BaseItem *item5 = new MagicBook;
	sherlockBag.insert(item1);
	sherlockBag.insert(item2);
	sherlockBag.insert(item3);
	sherlockBag.insert(item4);
	sherlockBag.insert(item5);
	cout << sherlockBag.str() << endl;
	cout << "Sherlock use an item : ";
	if(sherlockBag.get(ENERGY_DRINK) != NULL) cout << "Used" << endl;
	else cout << endl;
	cout << sherlockBag.str() << endl;
	cout << "Sherlock used " << sherlockBag.get(MAGIC_BOOK)->str() << endl;
	cout << sherlockBag.str() << endl;
	if (sherlockBag.get(PASSING_CARD) == NULL) cout << "Sherlock can not use Passing Card" << endl;
	cout << sherlockBag.str() << endl;
	cout << endl;
	
	
	// Test watsonBag
	WatsonBag watsonBag(&watson);
	watsonBag.insert(robots.getItemInside());
	cout << "Watson get an item from robotS: " << robots.getItemInside()->str() << endl;
	watsonBag.insert(robotsw.getItemInside());
	cout << "Watson get an item from robotSW: " << robotsw.getItemInside()->str() << endl;
	BaseItem *item10 = new PassingCard("RobotSW");
	BaseItem *item20 = new ExcemptionCard;
	BaseItem *item30 = new MagicBook;
	BaseItem *item40 = new PassingCard("RobotC");
	BaseItem *item50 = new MagicBook;
	watsonBag.insert(item10);
	watsonBag.insert(item20);
	watsonBag.insert(item30);
	watsonBag.insert(item40);
	watsonBag.insert(item50);
	cout << watsonBag.str() << endl;
	cout << "Watson use an item : ";
	if(watsonBag.get(ENERGY_DRINK) != NULL) cout << "Used" << endl;
	else cout << endl;
	cout << watsonBag.str() << endl;
	cout << "Watson used " << watsonBag.get(MAGIC_BOOK)->str() << endl;
	cout << watsonBag.str() << endl;
	if (watsonBag.get(EXCEMPTION_CARD) == NULL) cout << "Watson can not use Excemption Card" << endl;
	cout << watsonBag.str() << endl;
	cout << endl;
	
	

	// Test configuration
	const string filepath = "sa_tc_02_config";
	Configuration config(filepath);
	cout << config.str();
}