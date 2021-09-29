// Володин Александр ИУ8-53
// "Cyrus-Beck 2D Line Clipping algorithm"


struct point_2D // создадим структуру для нашей точки
{
    float x,y; // координаты нашей точки
};

// создаюм функцию для вычисления скалярного произведения 2-ух векторов
float DotProduct (point_2D vec1, point_2D vec2)
{
    return vec1.x*vec2.x + vec1.y*vec2.y;
}

const int max_p = 100; // масимальное кол-во точек
const int max_n = 100;

typedef point_2D Normal[max_n]; // создаем нормаль к плоскости

struct Polygon // создадим структуру "многоугольник"
{
    int n_points; // кол-во вершин(точек) многоугольника
    point_2D vec[max_p]; // координаты всех точек
};

void CalcNormals (Polygon poly, Normal & normal)
{ int j,k;
    point_2D vec;
    // чтобы все работало корректно, многоугольник должен быть выпуклым
    for (int i = 0; i < poly.n_points; i++)
    { j = (i+1)%poly.n_points;
        k = (i+2)%poly.n_points;

        normal[i].x = -(poly.vec[j].y - poly.vec[i].y)/(poly.vec[j].x - poly.vec[i].x);
        normal[i].y = 1.0;
        vec.x = poly.vec[k].x - poly.vec[i].x;
        vec.y = poly.vec[k].y - poly.vec[i].y;
        if (DotProduct(normal[i],vec) > 0)    // вычисляем нормаль
        { normal[i].x *= -1;
            normal[i].y  = -1;
        }
    }
}

void CyrusBeckAlgorithm (point_2D p1, point_2D p2, Normal normal, Polygon poly, bool & vision,
             point_2D & p3, point_2D & p4)
{ float t1,t2,t,dp1,dp2;
    point_2D dir_vec; // направляющий вектор
    point_2D vec;

// начинаем с максимальной стоимости из всех минимальных  и
// наименьшей стоимости из всех максимальных
    t1 = 0.0;
    t2 = 1.0;

    dir_vec.x = p2.x - p1.x;
    dir_vec.y = p2.y - p1.y;

    vision = true;
    int i = 0;
    while ( (i < poly.n_points) && vision)
    { vec.x = p1.x - poly.vec[i].x;
        vec.y = p1.y - poly.vec[i].y;

        dp1  = DotProduct (normal[i],vec);
        dp2   =  DotProduct (normal[i],dir_vec);

        if (dp2 == 0.0) // проверяем - параллель или точка пересечения
        { // если параллель, то смотрим внутри/снаружи области
            if (dp1 > 0.0)
                vision = false;
        }
        else
        { t = -(dp1/dp2);
            if (dp2 < 0.0) // если входит в области отсечения
            { if (t <= 1.0)
                    if (t > t1)
                        t1 = t;
            }
            else if ( t >= 0.0) // если находится вне области отсечения
                if (t < t2)
                    t2 = t;
        }
        i++;
    }
    if ( t1 <= t2)
    { p3.x = p1.x + t1*dir_vec.x;
        p3.y = p1.y + t1*dir_vec.y;
        p4.x = p1.x + t2*dir_vec.x;
        p4.y = p1.y + t2*dir_vec.y;
    }
    else
        vision = false;
}
int main()
{
    Normal normal;
    bool vision = true;
    point_2D p1,p2,p3,p4;
    p1.x = 1; p1.y = 2;
    p2.x = 5; p2.y = 4;
    p3.x = 3; p3.y = 6;
    p4.x = 2; p4.y = 8;
    Polygon poly;
    poly.n_points = 4;
    CalcNormals(poly,normal);
    CyrusBeckAlgorithm(p1,p2,normal,poly,vision,p3,p4);
    return 0;
}