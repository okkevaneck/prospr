/* File:       Protein.h
 * Author:     Okke van Eck
 *
 * Description:    Header file for a Protein object within the HP-model.
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
        std::vector<int> get_last_pos();
        std::vector<int> get_amino(std::vector<int> position);
        int get_score();
        int get_changes();
        void reset();
        void reset_conformation();
        bool is_valid(int move);
        void place_amino(int move, bool track=true);
        void remove_amino();
        void change_score(int move, int value);
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
