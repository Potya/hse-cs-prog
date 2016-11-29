// Сегодня мы сначала рассмотрим так называемые адаптеры над стандартными контейнерами.
// Это не самостоятельные контейнеры.
// Они используют какой-нибудь другой контейнер для хранения элементов (например, std::deque),
// но предоставляют свой интерфейс (набор функций для работы с контейнером).

// Первый такой адаптер - std::stack.
// Стек - это последовательность элементов, в которую разрешается добавлять элементы в конец и извлекать с конца (с вершины).
// Обращение к промежуточным элементам не допускается.
// Говорят, что структура данных "стек" реализует идею LIFO (last in - first out).

#include <iostream>
#include <stack>

int main() {
    std::stack<int> s;
    s.push(1);
    s.push(13);
    s.pop();  // удалить элемент с вершины стека (в нашем стеке останется 1)

    std::cout << s.top() << "\n";  // получить элемент на вершине стека, не удаляя его

    if (s.empty()) {  // проверка на пустоту
       //...
    }
}

// По умолчанию стек использует контейнер std::deque для хранения элементов.
// При желании мы можем указать другой тип этого контейнера.
// Всё, что требуется от этого контейнера, это поддержка операций
// push_back, pop_back, back и empty:

#include <iostream>
#include <list>
#include <stack>

int main() {
    std::stack<int, std::list<int>> s;  // используем std::list для хранения элементов в стеке
    // ...
}


// Другой похожий контейнер - очередь (std::queue).
// Очередь реализует идею FIFO (first in - first out).
// Можно считать, что элементы встают в очередь с одного конца, а извлекаются с другого.

#include <iostream>
#include <queue>

int main() {
    std::queue<int> s;
    s.push(1);
    s.push(13);
    s.pop();  // в очереди останется 13

    std::cout << s.front() << "\n";

    if (s.empty()) {
       //...
    }
}


// Ещё один адаптер - это очередь с приоритетами ("пирамида", "куча").
// Она позволяет за логарифмическое время добавлять и удалять элементы, и за константное время получать максимальный элемент.
// Детали реализациии кучи вам расскажут в следующем модуле на курсе алгоритмов.
// Сейчас давайте просто научимся работать с этим адаптером.

#include <iostream>
#include <queue>  // именно queue, а не priority_queue

int main() {
     std::priority_queue<int> pq;
     for (int x : {3, 14, 15, 92, 6})
         pq.push(n);
     while (!pq.empty()) {
         std::cout << pq.top() << "\n";
         pq.pop();
     }
     // Будет напечатано 92 15 14 6 3
}

// По умолчанию для сравнения элементов используется оператор <.
// Вы можете переопределить его для своих типов данных или вообще указать свой класс-компаратор.
// Примеры ниже.

// Строим кучу, на вершине которой будет лежать минимальный элемент:

#include <iostream>
#include <deque>
#include <functional>
#include <queue>

int main() {
     std::priority_queue<
         int,
         std::deque<int>,  // тип внутреннего контейнера; его приходится указывать, если нам нужен третий параметр
         std::greater<int>  // вспомогательный класс-обертка над оператором > (а не <)
     > pq;
     for (int x : {3, 14, 15, 92, 6})
         pq.push(n);
     while (!pq.empty()) {
         std::cout << pq.top() << "\n";
         pq.pop();
     }
     // Будет напечатано 3 6 14 15 92
}

// Можно определить компаратор для своей структуры и так:

struct Time {
    int h, m, s;
};

// Можно, конечно, просто определить оператор <:
/*bool operator < (const Time& a, const Time& b) {
    return a.h < b.h || a.h == b.h && a.m < b.m || a.h == b.h && a.m == b.m && a.s < b.s;
}*/

// А можно и так:
struct TimeComparator {
    bool operator() (const Time& a, const Time& b) const {
        return a.h < b.h || a.h == b.h && a.m < b.m || a.h == b.h && a.m == b.m && a.s < b.s;
    }
};

int main() {
    std::priority_queue<
        Time,
        std::deque<Time>,
        TimeComparator
    > pq;
}


// Продолжим изучение алгоритмов стандартной библиотеки.
// Рассмотрим алгоритм std::remove.
// Важно понимать, что алгоритмы работают с итераторами,
// и поэтому они ничего не знают о физическом способе хранения элементов в контейнера.
// Алгоритмы (благодаря итераторам) знают лишь о логическом порядке перебора элементов.
// Поэтому алгоритмы не могут физически что-либо удалить из контейнера (или добавить в него).
// Лучшее, что они могут сделать - это переупорядочить элементы в последовательности.

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {5, 13, 12, 5, -293, 10};

    auto iter = std::remove(v.begin(), v.end(), 5);  // хотим избавиться от пятёрки
    // В итоге в векторе будет примерно следующее:
    // 13, 12, -293, 10, -293, 10
    //                     ^ сюда будет указывать iter - это правая граница переупорядоченного полуинтервала
    //                       после этой границы в контейнере останутся какие-то элементы (мусор)

    // Физически удалить кусок ненужных оставшихся элементов можно так:
    v.erase(iter, v.end());

    for (int x : v)
        std::cout << x << " ";  // теперь будет напечатано 13 12 -293 10
    std::cout << "\n";
}

// Другой алгоритм, переупорядочивающий элементы - std::unique.
// Он работает примерно как стандартная консольная утилита uniq.
// Если в последовательности имеется несколько подряд идущих одинаковых элементов, то из них остаётся только один.

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {5, 5, 13, 12, 5, -293, 10};
    auto iter = std::unique(v.begin(), v.end());
    // В векторе будет 5, 13, 12, 5, -293, 10
    //                                     ^ iter будет указывать сюда
    v.erase(iter, v.end());
}

// Обратите внимание, что третья пятёрка не удалилась, потому что она не расположена рядом с другими пятёрками.
// Обычно std::unique применяют вместе с std::sort, чтобы одинаковые элементы оказались рядом:

std::sort(v.begin(), v.end());
v.erase(std::unique(v.begin(), v.end()), v.end());


// Особый класс алгортмов - алгоритмы для работы с отсортированными последовательностями
// Например, в таких последовательностях можно выполнять бинарный поиск:

#include <algorithm>
#include <iostream>
#include <list>

int main() {
    std::list<int> data = {1, 4, 5, 9, 9, 13, 47};
    std::cout << std::binary_search(data.begin(), data.end(), 5) ? "Found\n" : "Not found\n";

    // Если binary_search возвращает true/false,
    // то алгоритмы lower_bound и upper_bound возвращают итераторы.
    // Алгоритм lower_bound возвращает итератор на первый элемент, не меньший заданного:
    auto iter1 = std::lower_bound(data.begin(), data.end(), 8);  // *iter1 == 9
    // Алгоритм upper_bound возвращает итератор на первый элемент, больший заданного:
    auto iter2 = std::upper_bound(data.begin(), data.end(), 50);  // iter2 == data.end()
    // все элементы исходной последовательности, такие, что 8 <= x <= 50, попадут в полуинтервал [iter1, iter2)

    if (iter1 != data.end() && *iter1 == 8)
        std::cout << "FOUND\n";
}


// Теоретико-множественные операции над отсортированными последовательностями
// можно делать с помощью алгоритмов
// set_intersection, set_union, set_difference, set_symmetric_difference и includes.
// Эти алгоритмы работают за линейное время от суммарной длины последовательностей.
// Обратите внимание, что элементы для этого совсем не обязательно должны храниться в std::set.
// Они могут храниться в любом последовательном контейнере.
// Важно лишь, чтобы они были отсортированными.
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 3, 5, 7, 9}, v2 = {1, 2, 3, 4};
    std::vector<int> out(v1.size());  // вектор достаточного размера для записи результатов

    auto iter = set_intersection(  // общие элементы последовательностей
        v1.begin(),
        v1.end(),
        v2.begin(),
        v2.end(),
        out.begin()
    );

    out.erase(iter, out.end());  // удаляем лишние элементы

    for (int x : out)
        cout << x << " ";
    std::cout << "\n";
    // Будет напечатано 1 3
}

