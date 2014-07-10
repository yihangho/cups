var VOLUME_X = 4;
var VOLUME_Y = 9;
var TARGET_VOLUME = 2;

if (process.argv.length >= 5) {
    VOLUME_X = parseInt(process.argv[2]);
    VOLUME_Y = parseInt(process.argv[3]);
    TARGET_VOLUME = parseInt(process.argv[4]);
}

function Cups(x, y) {
    this.x = x;
    this.y = y;

    if (typeof this.constructor.prototype.fill_x != 'function') {
        this.constructor.prototype.toString = function() {
            return "(" + this.x + ", " + this.y + ")";
        };

        this.constructor.prototype.fulfill_target = function() {
            return this.x == TARGET_VOLUME || this.y == TARGET_VOLUME;
        };

        this.constructor.prototype.fill_x = function() {
            return new this.constructor(VOLUME_X, this.y);
        };

        this.constructor.prototype.fill_y = function() {
            return new this.constructor(this.x, VOLUME_Y);
        };

        this.constructor.prototype.empty_x = function() {
            return new this.constructor(0, this.y);
        };

        this.constructor.prototype.empty_y = function() {
            return new this.constructor(this.x, 0);
        };

        this.constructor.prototype.pour_x_y = function() {
            var new_y = Math.min(VOLUME_Y, this.x + this.y);
            var new_x = this.x - (new_y - this.y);
            return new this.constructor(new_x, new_y);
        };

        this.constructor.prototype.pour_y_x = function() {
            var new_x = Math.min(VOLUME_X, this.x + this.y);
            var new_y = this.y - (new_x - this.x);
            return new this.constructor(new_x, new_y);
        };
    }
}

function CupState(steps, parent) {
    this.steps = steps;
    this.parent = parent;
}

var cup_states_container = {
    child_state: function(parent) {
            if (typeof this[parent] == 'undefined') {
                return null;
            }

            return new this[parent].constructor(this[parent].steps + 1, parent);
        }
};
cup_states_container[new Cups(0 ,0)] = new CupState(0, null);
var bfs_queue = [new Cups(0, 0)];
var mutation_methods = ["fill_x", "fill_y", "empty_x", "empty_y", "pour_x_y", "pour_y_x"];

while (bfs_queue.length != 0) {
    var cur_cups = bfs_queue.shift();
    if (cur_cups.fulfill_target()) {
        console.log("Found an answer with " + cup_states_container[cur_cups].steps + " steps!");
        var ans_arr = []
        while (cur_cups) {
            ans_arr.push(cur_cups);
            cur_cups = cup_states_container[cur_cups].parent;
        }
        ans_arr.reverse();
        for (var i = 0; i < ans_arr.length; i++) {
            console.log(ans_arr[i].toString());
        }
        break;
    }
    mutation_methods.forEach(function(sym) {
        var next_cups = cur_cups[sym]();
        if (typeof cup_states_container[next_cups] == 'undefined') {
            cup_states_container[next_cups] = cup_states_container.child_state(cur_cups);
            bfs_queue.push(next_cups);
        }
    });
}
