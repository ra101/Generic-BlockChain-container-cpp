#include <iostream>

namespace ra
{
    class not_implemented_exception : public std::logic_error
    {
    public:
        not_implemented_exception(
            std::string function = __builtin_FUNCTION()) : std::logic_error("`" + function + "` not implemented!"){};
    };

}
