import re
import sys


def getStreetInformation(command):

    coordinate = re.findall(r'\(-?\d+,-?\d+\)', command[2])
    # print coordinate
    streetName = command[1]
    for i in range(len(coordinate)):
        coordinate[i] = re.findall(r'-?\d+', coordinate[i])
        coordinate[i][0] = int(coordinate[i][0])
        coordinate[i][1] = int(coordinate[i][1])

    return [streetName, coordinate]


class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)
        self.index = 0

    def __str__(self):
        return '({0:.2f}, {1:.2f})'.format(self.x, self.y)

    def __repr__(self):
        return '({0:.2f}, {1:.2f})'.format(self.x, self.y)


class Line(object):
    def __init__(self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)

    def __repr__(self):
        return '({}-->{})'.format(self.src, self.dst)

    def add_point_to_line(self, p1):

        insertPoint = []
        newLine = []
        if len(p1) == 0:
            return [self]
        insertPoint = list(set([self.src] + p1 + [self.dst]))
        A, B, C = GeneralEquation(self.src, self.dst)
        if A == 0 and B is not 0:
            insertPoint = sorted(insertPoint, key=lambda pointx: pointx.x)
        if B == 0 and A is not 0:
            insertPoint = sorted(insertPoint, key=lambda pointy: pointy.y)
        if A * B <= 0:
            insertPoint = sorted(insertPoint, key=lambda pointx: pointx.x)
        else:
            insertPoint = sorted(insertPoint, key=lambda pointy: pointy.y)

        for i in range(len(insertPoint) - 1):
            if str(insertPoint[i]) == str(insertPoint[i + 1]):
                continue
            newLine.append(Line(insertPoint[i], insertPoint[i + 1]))

        return newLine


def GeneralEquation(p1, p2):
    A = p2.y - p1.y
    B = p1.x - p2.x
    C = p2.x * p1.y - p1.x * p2.y
    return A, B, C


def IsInLine(line, point):
    A, B, C = GeneralEquation(line.src, line.dst)
    if A * point.x + B * point.y + C == 0:
        if point.x <= line.dst.x and point.x >= line.src.x:
            return True
        if point.x <= line.src.x and point.x >= line.dst.x:
            return True
    return False


def intersect(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y
    A1, B1, C1 = GeneralEquation(l1.src, l1.dst)
    A2, B2, C2 = GeneralEquation(l2.src, l2.dst)
    m = A1 * B2 - A2 * B1
    if m == 0:
        if x1 == x3 and x2 is not x4:
            return Point(x1, y1)
        if x1 == x4 and x2 is not x3:
            return Point(x1, y1)
        if x2 == x3 and x1 is not x4:
            return Point(x2, y2)
        if x2 == x4 and x1 is not x3:
            return Point(x2, y2)

        return
    xcoor = (C2 * B1 - C1 * B2) / m
    ycoor = (C1 * A2 - C2 * A1) / m
    if min(x1, x2) <= xcoor and xcoor <= max(x1, x2) and min(x3, x4) <= xcoor and xcoor <= max(x3, x4):
        if min(y1, y2) <= ycoor and ycoor <= max(y1, y2) and min(y3, y4) <= ycoor and ycoor <= max(y3, y4):
            return Point(xcoor, ycoor)
    return


def getLinePoint(strData):
    point = []

    for i in range(len(strData)):
        str_point = []
        for j in range(len(strData[i][1])):
            str_point.append(Point(strData[i][1][j][0], strData[i][1][j][1]))
        point.append(str_point)

    return point


def getLine(point):
    line = []
    # flag = []
    for i in range(len(point)):
        str_line = []
        # flag_line = []
        for j in range(len(point[i]) - 1):
            str_line.append(Line(point[i][j], point[i][j + 1]))
            # flag_line.append(0)

        line.append(str_line)
        # flag.append(flag_line)
    return line

    # return line, flag


def findVertex(strData):
    vertex = []
    point = getLinePoint(strData)
    line = getLine(point)
    # line, flag = getLine(point)
    intersection = []
    vertex_line = []

    for i in range(len(line) - 1):
        for j in range(len(line[i])):
            for m in range(i + 1, len(line)):
                for k in range(len(line[m])):
                    if intersect(line[i][j], line[m][k]) is not None:
                        vertex_line.append(line[i][j])
                        vertex_line.append(line[m][k])
                        intersection.append(intersect(line[i][j], line[m][k]))
                        vertex.append(intersect(line[i][j], line[m][k]))
                        vertex.append(line[i][j].dst)
                        vertex.append(line[m][k].dst)
                        vertex.append(line[i][j].src)
                        vertex.append(line[m][k].src)
    vertex = delete_duplicate_elem(vertex)
    intersection = delete_duplicate_elem(intersection)
    vertex_line = delete_duplicate_elem(vertex_line)

    return vertex, line, intersection, vertex_line


def delete_duplicate_elem(l):
    for i in range(len(l) - 1):
        for j in range(i + 1, len(l)):
            if str(l[i]) == str(l[j]):
                l[i] = l[j]
    l = list(set(l))
    return l


def findEdge(vertex_line, intersection):
    edge = []
    new_line = []
    for i in vertex_line:
        add_intersect_to_line = []
        for m in intersection:
            if IsInLine(i, m):
                add_intersect_to_line.append(m)

        new_line = i.add_point_to_line(add_intersect_to_line)
        for h in new_line:
            edge.append(h)

    return edge


def graphPrinter(vertex, edge):
    edge_to_graph = []
    sys.stdout.write("V "+str(len(vertex))+"\n")

    # print("V = {")
    for i in range(len(vertex)):
        vertex[i].index = i
    #     print("  " + str(i+1) + ":" + " " +str(vertex[i]))
    # print("}")

    for i in range(len(edge)):
        edge1 = []
        for v in range(len(vertex)):
            if str(edge[i].src) == str(vertex[v]):
                edge1.append(vertex[v].index)
            if str(edge[i].dst) == str(vertex[v]):
                edge1.append(vertex[v].index)
        edge_to_graph.append(edge1)

    sys.stdout.write("E {")

    for i in range(len(edge_to_graph)):
        if len(edge_to_graph) == 0:
            sys.stdout.write("}")
            break
        if i == len(edge_to_graph) - 1:
            sys.stdout.write("<" + str(edge_to_graph[i][0]) + "," + str(edge_to_graph[i][1]) + ">")
            continue
        sys.stdout.write("<" + str(edge_to_graph[i][0]) + "," + str(edge_to_graph[i][1]) + ">,")

    sys.stdout.write("}\n")
    sys.stdout.flush();

    # print("E = {")
    # for i in range(len(edge_to_graph)):
    #     if len(edge_to_graph) == 0:
    #         print("}")
    #         break
    #     if i == len(edge_to_graph) - 1:
    #         print("   <" + str(edge_to_graph[i][0]) + "," + str(edge_to_graph[i][1]) + ">")
    #         continue
    #     print("   <" + str(edge_to_graph[i][0]) + "," + str(edge_to_graph[i][1]) + ">,")
    #
    # print("}")

#
# def valid(string):
#     match = re.match(
#         "([a]\\s+\"[a-zA-Z ]+\"\\s+(\\(\\-?(0|[1-9][0-9]*),\\-?(0|[1-9][0-9]*)\\)\\s*)+)|r{1}|g{1}",
#         string)
#     if not match:
#         return False
#     else:
#         return True


def parse_line(input,strData):
    command = re.split("\s\"|\"\s",input)

    sub = command[0][0]
    # print sub
    if sub == "r":

        flag_remove = True
        for i in range(len(strData)):
            if strData[i][0] == command[1]:
                del strData[i]
                flag_remove = False
                break;
        if flag_remove:
            raise Exception('Error: \'r\' specified for a street that does not exist')
        # del strData[:]

    if sub == "a":
        str = getStreetInformation(command)
        if len(str[0]) == 0:
            raise Exception('Error: unvalid street name input for \'a\' command')
        if len(str[1]) <= 1:
            raise Exception('Error: unvalid street coordianate input for \'a\' command')

        for i in range(len(strData)):
            if strData[i] == str:
                raise  Exception('Error: street currently exists.')
        strData.append(str)

    if sub == "g":
        vertex, line, intersection, vertex_line = findVertex(strData)
        edge = findEdge(vertex_line, intersection)
        graphPrinter(vertex, edge)



def main():

    while True:
        line = sys.stdin.readline()
        if line == '':
            break

        parse_line(line,strData)

    sys.exit(0)


if __name__ == '__main__':
    strData = []
    main()
