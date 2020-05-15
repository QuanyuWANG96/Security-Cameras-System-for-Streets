#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
#include <math.h>


typedef struct Point{
    int x;
    int y;
    bool equal(Point p1);
}Point;
typedef struct Line{
    Point src;
    Point dst;
}Line;
//
//int genRanNum(int left, int right){
//    int value;
//    value = rand() % (right - left +1) +left;
//    return value;
//}


int genRanNum(int low, int high) {
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    unsigned int num;
    urandom.read((char *) &num, sizeof(int));
    urandom.close();
    signed int value = low + (num % (high - low + 1));
    return value;
}
//
//int genRanNum(int left, int right){
//
//    int num , value;
//    std::ifstream urandom("/dev/urandom");
//    char ch ;
//    urandom.read(&ch, 1);
//    num = (unsigned int) ch;
//    value = left + num % (right - left + 1);
//    std::cout << "value   " << value<<std::endl;
//    urandom.close();
//    return value;
//}


Point getPoint(int ran){
    Point p ;
    p.x = genRanNum(-ran,ran);
    p.y = genRanNum(-ran,ran);
    return p;
}
bool isInLine(Point p1, Line l1){
    if (p1.x == l1.src.x) return p1.y == l1.src.y;
    if (p1.x == l1.dst.x) return p1.y == l1.dst.y;
    return (l1.src.x - p1.x)*(l1.dst.y - p1.y) == (l1.dst.x - p1.x)*(l1.src.y - p1.y);
}

bool checkPoint(Point p1, std::vector<Line> &line){
    for(int i = 0; i<line.size(); i++){
        if (isInLine(p1, line[i])){
            return false;
        }
    }
    return true;
}

std::vector<Point> genStrLine(int seg, int ran, std::vector<Line> &line){
    int num_edge;
    num_edge = genRanNum(1,seg);
    std::vector<Point>  strPoint;

    for (int i = 0; i < num_edge +1 ; i++){
        Point p = getPoint(ran);
        for(int j = 0; j<=25; j++){
            if(checkPoint(p,line)){
                break;
            }
            p = getPoint(ran);
            if(j == 25){
                std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts\n";
            //    break;
                exit(0);

            }
        }
        strPoint.push_back(p);
        if(i>0){
            Line newline;
            newline.src = strPoint[i-1];
            newline.dst = p;
            line.push_back(newline);
        }
    }
    return strPoint;
}

std::vector<std::vector<Point>> genStreet(std::vector<int> k, std::vector<Line> &line ){
    int num_s;
    num_s = genRanNum(2,k[0]);
    std::vector<std::vector<Point>> street;

    for (int i = 0 ; i<num_s; i++){
        std::vector<Point> s = genStrLine(k[1], k[3], line);
        street.push_back(s);
    }

    return street;
}


int main(int argc, char **argv){
    int c;
    std::vector<int> k = {10, 5, 5, 20};
    static const char alphanum[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    opterr = 0;
    while ((c = getopt (argc, argv, ":s:n:l:c:")) != -1)
        switch (c)
        {
            case 's':
                k[0]= atoi(optarg);
                break;
            case 'n':
                k[1]= atoi(optarg);
                break;
            case 'l':
                k[2]= atoi(optarg);
                break;
            case 'c':
                k[3]= atoi(optarg);
                break;
            case ':':
                break;
            case '?':
                if (optopt == 'c')
                    std::cerr << "Error: option -" << optopt
                              << " requires an argument." << std::endl;
                else
                    std::cerr << "Error: unknown option: " << optopt << std::endl;
                return 1;
            default:
                return 0;
        }

    while(1){
        std::vector<Line> line;
        std::vector<std::vector<Point>> street = genStreet(k,line);
        std::vector<std::string>  streetName;

        //a
        for(int i = 0; i<street.size(); i++){
            std::cout << "a \"";
            int num;
            std::string s;
            for (int j = 0; j < 8; j++) {
                num = genRanNum(1,52);
                s = s + alphanum[num];
            }
            streetName.push_back(s);
            std::cout << s<<"\" ";
            for (int g=0; g <street[i].size(); g++){
                std::cout << "(" << street[i][g].x << "," << street[i][g].y << ")  ";
            }
            std::cout << std::endl;
        }

        //g
        std::cout <<"g"<< std::endl;

        //r
        for(int i = 0; i<streetName.size(); i++){
            std::cout << "r \"" << streetName[i] << "\""<<std::endl;
        }

        street.clear();
        line.clear();

        int waitTime = genRanNum(5,k[2]);
        sleep(waitTime);

    }

    return 0;
}

