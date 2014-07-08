<?php
class CupState
{
	public static function initializeEvent($cup_capacities , $vol_target){
		CupState::$target_volume = $vol_target;
		CupState::$cup_max_volumes = array();
		$i = 0;
		foreach($cup_capacities as $volume){
			CupState::$cup_max_volumes[$i] = $volume;
			$i += 1;
		}
		CupState::$number_of_cups = $i;
	}

	public function __construct($collection_of_cup_volumes){
		$this->cup_volumes = array();
		foreach($collection_of_cup_volumes as $volume){
			array_push($this->cup_volumes , $volume);
		}
	}

	public function hash(){
		return ($this->cup_volumes[0]) . "," . ($this->cup_volumes[1]);
	}

	public function isTargetVolume(){
		foreach($this->cup_volumes as $volume)
			if($volume == CupState::$target_volume)
				return true;
		return false;
	}

	public function getCupVolume($id){
		if($id > CupState::$number_of_cups) throw new Exception("Invalid Cup Id");
		return $this->cup_volumes[$id];
	}

	public function pour($source , $dest){
		$new_cup_volumes = $this->cup_volumes;
		$new_cup_volumes[$dest] += $new_cup_volumes[$source];
		$new_cup_volumes[$dest] = min(CupState::$cup_max_volumes[$dest] , $new_cup_volumes[$dest]);
		$new_cup_volumes[$source] -= ($new_cup_volumes[$dest] - $this->cup_volumes[$dest]);
		return new CupState($new_cup_volumes);
	}

	public function clear($cup_id){
		$new_cup_volumes = $this->cup_volumes;
		$new_cup_volumes[$cup_id] = 0;
		return new CupState($new_cup_volumes);
	}

	public function fill($cup_id){
		$new_cup_volumes = $this->cup_volumes;
		$new_cup_volumes[$cup_id] = CupState::$cup_max_volumes[$cup_id];
		return new CupState($new_cup_volumes);
	}

	public $parent;
	private $cup_volumes;
	private static $number_of_cups;
	private static $target_volume;
	private static $cup_max_volumes;
}


CupState::initializeEvent(array(isset($argv[1]) ? $argv[1] : 9, isset($argv[2]) ? $argv[2] : 4),isset($argv[3]) ? $argv[3] : 2);
$visited_states = array();
$bfs_queue = array();

$cup_state = new CupState(array(0,0));
$cup_state->parent = false;

$achieved_state = false;

array_push($bfs_queue , $cup_state);
while($bfs_queue){

	$current_state = $bfs_queue[0];
	array_shift($bfs_queue);
	$new_possible_state;

	if(isset($visited_states[$current_state->hash()]))
		continue;

	$visited_states[$current_state->hash()] = true;

	if($current_state->isTargetVolume()){
		$achieved_state = true;
		$messages = array();
		do {
			array_push($messages , $current_state->getCupVolume(0) . ", " . $current_state->getCupVolume(1) . "\n");
			$current_state = $current_state->parent;
		}while($current_state);
		for($i = count($messages) - 1 ; $i >= 0 ; $i--)
			echo $messages[$i];
		break;
	}

	for($i = 0 ; $i < 6 ; $i++){
		if($i == 0) $new_possible_state = $current_state->fill(0);
		else if($i == 1) $new_possible_state = $current_state->fill(1);
		else if($i == 2) $new_possible_state = $current_state->clear(0);
		else if($i == 3) $new_possible_state = $current_state->clear(1);
		else if($i == 4) $new_possible_state = $current_state->pour(0,1);
		else if($i == 5) $new_possible_state = $current_state->pour(1,0);

		if(!isset($visited_states[$new_possible_state->hash()])){
			$new_possible_state->parent = $current_state;
			array_push($bfs_queue , $new_possible_state);
		}
	}
}

if(!$achieved_state)
	echo "Impossible to reach that state!\n";