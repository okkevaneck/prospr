/* File:            protein.h
 * Description:     Header file for a protein object within the HP-model.
 */

#include <string.h>
#include <map>
#include <vector>


class Protein {
    public:
        Protein(std::string sequence, int dim=2);
        std::string get_sequence();
        int get_dim();
        int get_cur_len();
        std::map<std::vector<int>, std::vector<int>> get_space();
        int get_last_move();
        std::vector<int> get_last_pos();
        std::vector<int> get_amino(std::vector<int> position);
        int get_score();
        int get_changes();
        std::vector<int> get_h_idxs();
        bool is_hydro(int index);
        void reset();
        void reset_conformation();
        bool is_valid(int move);
        void place_amino(int move, bool track=true);
        // TODO: Change function to use the last_move attribute.
        void remove_amino(int move);
        void change_score(int move, int weight);
        std::vector<int> hash_fold();
        void set_hash(std::vector<int> fold_hash, bool track=false);

    private:
        std::string sequence;
        std::vector<int> h_idxs;
        std::map<std::vector<int>, std::vector<int>> space;
        int cur_len;
        int dim;
        int last_move;
        std::vector<int> last_pos;
        int score;
        int changes;
};
