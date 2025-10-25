#include "core/history_manager.hpp"
#ifdef __APPLE__
#include "core/stdc++.hpp"
#elif defined(__MACH__)
#include "stdc++.hpp"
#else
#include <bits/stdc++.h>
#endif

#include "ui/main_screen.hpp"
#include "core/config_manager.hpp"
#include "core/localization.hpp"

#include "calc.hpp"

// double eval_func(const std::string &expr)
// {
//     std::stringstream ss(expr);
//     double result = 0;
//     double value = 0;
//     char op = '+';
//     while (ss >> value)
//     {
//         switch (op)
//         {
//         case '+':
//             result += value;
//             break;
//         case '-':
//             result -= value;
//             break;
//         case '*':
//             result *= value;
//             break;
//         case '/':
//             if (value != 0)
//                 result /= value;
//             break;
//         }
//         ss >> op;
//     }
//     return result;
// }

int main()
{
    ConfigManager config("fast_calc");
    LocalizationManager localization("lang");
    HistoryManager manager;
    MainScreen app(eval_func, config, localization, manager);
    app.Run();
    return 0;
}
