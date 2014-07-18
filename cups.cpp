#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

class CupState
{
public:
    CupState(const ll & vol_1 = 0 , const ll & vol_2 = 0);
    static void initializeEvent(const ll & vol_1 , const ll & vol_2 , const ll & target_vol);
    ll getCupVolume(const short & cup_id);
    CupState pour(const short & source_id , const short & target_id);
    CupState fill(const short & cup_id);
    CupState empty(const short & cup_id);
    short isTargetVolume();
    string hash();
    static ll target_volume;
    static ll cup_1_max_volume;
    static ll cup_2_max_volume;
    CupState * ptr_parent;
private:
    ll cup_1_volume;
    ll cup_2_volume;
};

ll CupState::target_volume;
ll CupState::cup_1_max_volume;
ll CupState::cup_2_max_volume;

CupState::CupState(const ll & vol_1 , const ll & vol_2 ){
    cup_1_volume = vol_1;
    cup_2_volume = vol_2;
    ptr_parent = NULL;
}

void CupState::initializeEvent(const ll & vol_1 , const ll & vol_2 , const ll & target_vol){

    CupState::cup_1_max_volume = vol_1;
    CupState::cup_2_max_volume = vol_2;
    CupState::target_volume = target_vol;
}

ll CupState::getCupVolume(const short & cup_id){
    if(cup_id == 1) return cup_1_volume;
    else if (cup_id == 2) return cup_2_volume;
    else return 0;
}

CupState CupState::pour(const short & source_id , const short & target_id){
    if(source_id == target_id) throw -1;

    if(source_id == 1 && target_id == 2){
        ll new_cup_2_volume = cup_2_volume;
        ll new_cup_1_volume = cup_1_volume;
        new_cup_2_volume += new_cup_1_volume;
        new_cup_2_volume = min(new_cup_2_volume , CupState::cup_2_max_volume);
        new_cup_1_volume -= (new_cup_2_volume - cup_2_volume);

        return CupState(new_cup_1_volume , new_cup_2_volume);
    }
    else if(source_id == 2 && target_id == 1){
        ll new_cup_1_volume = cup_1_volume;
        ll new_cup_2_volume = cup_2_volume;
        new_cup_1_volume += new_cup_2_volume;
        
        new_cup_1_volume = min(new_cup_1_volume , CupState::cup_1_max_volume);
        new_cup_2_volume -= (new_cup_1_volume - cup_1_volume);

        return CupState(new_cup_1_volume , new_cup_2_volume);
    }
    else
        throw "invalid cup id!";
}

CupState CupState::fill(const short & cup_id){
    if(cup_id == 1) return CupState(CupState::cup_1_max_volume , cup_2_volume);
    else if (cup_id == 2) return CupState(cup_1_volume , CupState::cup_2_max_volume);
    else throw "invalid cup id!";
}

CupState CupState::empty(const short & cup_id){
    if(cup_id == 1) return CupState(0 , cup_2_volume);
    else if (cup_id == 2) return CupState(cup_1_volume , 0);
    else throw "invalid cup id!";
}

short CupState::isTargetVolume(){
    if(cup_1_volume == CupState::target_volume) return 1;
    else if(cup_2_volume == CupState::target_volume) return 2;
    else return 0;
}

string CupState::hash(){
    stringstream ss;
    ss << cup_1_volume << "," << cup_2_volume;
    return ss.str();
}

int main(int argc , char * argv[])
{
    try{
        long long vol_1_tmp = argc >= 2 ? strtol(argv[1] , NULL, 10) : 90;
        long long vol_2_tmp  = argc >= 3 ? strtol(argv[2] , NULL, 10) : 40;
        long long target_vol_tmp = argc >= 4 ? strtol(argv[3] , NULL, 10) : 20;
        assert(vol_1_tmp >= 0 && vol_2_tmp >= 0 && target_vol_tmp >= 0);
        CupState::initializeEvent(vol_1_tmp , vol_2_tmp , target_vol_tmp);

        CupState::initializeEvent(vol_1_tmp , vol_2_tmp , target_vol_tmp);
        printf("Your target volume is %lld , max volume 1 is %lld whereas max volume 2 is %lld\n", CupState::target_volume , CupState::cup_1_max_volume , CupState::cup_2_max_volume );

        map<string , bool> visited_states;
        queue<CupState> bfs;
        bfs.push(CupState(0,0));
        bool found_volume = false;
        CupState * ptr_current_state;

        while(!bfs.empty()){
            
            ptr_current_state = new CupState;
            *ptr_current_state = bfs.front();
            bfs.pop();

            if(visited_states[ptr_current_state->hash()])
                continue;
            
            visited_states[ptr_current_state->hash()] = true;

            if(ptr_current_state->isTargetVolume()){
                found_volume = true;
                break;
            }

            CupState next;
            for(int i = 0 ; i < 6 ; i++){
                switch(i) {
                    case 0 : next = ptr_current_state->pour(1,2); break;
                    case 1 : next = ptr_current_state->pour(2,1); break;
                    case 2 : next = ptr_current_state->empty(1); break;
                    case 3 : next = ptr_current_state->empty(2); break;
                    case 4 : next = ptr_current_state->fill(1); break;
                    case 5 : next = ptr_current_state->fill(2); break;
                }
                if(!visited_states[next.hash()]){
                    next.ptr_parent = ptr_current_state;
                    bfs.push(next);
                }
            }
        }
        if(found_volume) {
            vector<pair<ll,ll> > results;
            do{
            	results.push_back(make_pair(ptr_current_state->getCupVolume(1) , ptr_current_state->getCupVolume(2)));
                ptr_current_state = ptr_current_state->ptr_parent;
            } while(ptr_current_state);
            
            for(int i = results.size() - 1 ; i >= 0 ; i--){
            	printf("(%lld , %lld)\n" , results[i].first , results[i].second);	
            }
        }
        else
            puts("No possible solution!\n");
    }
    catch(const char* e){
        printf("%s " , e);
    }

    return 0;
}