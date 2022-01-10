#include <fstream> //for file-handling

#define DB_SIZE 1000
#define DB_FILEPATH "includes/primeDB"

namespace ra
{

    template <typename Engine>
    class random_prime_engine
    {
        //this engine takes random engines as template input
        Engine eng;
        int min_prime;
        int max_prime;
        char const *db_filepath;

    public:
        random_prime_engine(unsigned long rand_seed = 0, char const *db_filepath = DB_FILEPATH);
        int operator()();
        template <class EngineKlass>
        friend std::stringstream &operator<<(std::stringstream &, const random_prime_engine<EngineKlass> &);
        template <class EngineKlass>
        friend std::stringstream &operator>>(std::stringstream &, random_prime_engine<EngineKlass> &);
        int min();
        int max();
    };

    template <typename Engine>
    random_prime_engine<Engine>::random_prime_engine(unsigned long rand_seed, char const *db_filepath)
    {
        //lazy intiallisation
        min_prime = 0;
        max_prime = 0;

        // Get DB path
        this->db_filepath = db_filepath;

        //
        eng.seed(rand_seed);
    }

    template <typename Engine>
    int random_prime_engine<Engine>::operator()()
    {
        int idx = eng() % DB_SIZE; //to get a index within file
        int rand_prime;
        std::ifstream file;
        file.open(db_filepath);

        //update rand_prime till it gets the value at index
        for (int i = 0; i <= idx; i++)
            file >> rand_prime;

        file.close();
        return (rand_prime);
    }

    template <typename Engine>
    int random_prime_engine<Engine>::min()
    {
        //if min==0 , i.e. first time intiallization
        if (!min_prime)
        {
            std::ifstream file;
            file.open(db_filepath);
            //get int at the start of file
            file >> min_prime;
            file.close();
        }

        return min_prime;
    }

    template <typename Engine>
    int random_prime_engine<Engine>::max()
    {
        //if max==0 , i.e. first time intiallization
        if (!max_prime)
        {
            std::ifstream file;
            file.open(db_filepath);
            //get int at the end of file
            while (file)
                file >> max_prime;
            file.close();
        }
        return max_prime;
    }

    template <class EngineKlass>
    std::stringstream &operator<<(std::stringstream &out, const random_prime_engine<EngineKlass> &prime_class)
    {
        //save the state
        out << prime_class.eng;
        return out;
    }

    template <class EngineKlass>
    std::stringstream &operator>>(std::stringstream &in, random_prime_engine<EngineKlass> &prime_class)
    {
        //restores the state
        in >> prime_class.eng;
        return in;
    }
}
