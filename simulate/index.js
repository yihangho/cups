function get_transformation_method(vol_x , vol_y , vol_target){
    var VOLUME_X = vol_x;
    var VOLUME_Y = vol_y;
    var TARGET_VOLUME = vol_target;

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
            var ans_arr = [];
            var return_arr = [];
            while (cur_cups) {
                return_arr.push([cur_cups.x , cur_cups.y]);
                ans_arr.push(cur_cups);
                cur_cups = cup_states_container[cur_cups].parent;
            }
            ans_arr.reverse();
            return_arr.reverse();

            for (var i = 0; i < ans_arr.length; i++) {
                console.log(ans_arr[i].toString());
            }

            console.log(cup_states_container);
            return return_arr;
        }
        mutation_methods.forEach(function(sym) {
            var next_cups = cur_cups[sym]();
            if (typeof cup_states_container[next_cups] == 'undefined') {
                cup_states_container[next_cups] = cup_states_container.child_state(cur_cups);
                bfs_queue.push(next_cups);
            }
        });
    }

    return false;
}

var print_notice = function(notice , positive , timing){
    if($("#alert_notice").css("display").toString().toLowerCase() != 'none'){
        $("#alert_notice").hide(timing);
    }
    document.getElementById("alert_notice").className = positive ? "alert alert-success" : "alert alert-danger";
    document.getElementById("alert_notice").innerHTML = notice;
    $("#alert_notice").slideDown(timing);
}

var simulateEvent = function(VOLUME_X , VOLUME_Y , TARGET_VOLUME , speed){
    var maximum_pixel_height = $(window).height() / 1.5;
    var cup_x_pixel_height;
    var cup_y_pixel_height;
    var run_event = false;
    var speed = 2000 - speed;

    var animate = function(depth , array_of_results){
        if(depth == 0){
            $("#main_presentation_screen").show();
        }
        if(depth == array_of_results.length){
            document.getElementById("trigger_event_button").disabled = false;
            return;
        }
        var x = array_of_results[depth][0];
        var y = array_of_results[depth][1];

        console.log(array_of_results[depth][0] + " " + array_of_results[depth][1]);
        console.log(($("#cup_x").height() - x / VOLUME_X * $("#cup_x").height()).toString() + " " + ($("#cup_y").height() - y / VOLUME_Y * $("#cup_y").height()).toString() );

        $("#cup_x").animate({"padding-top" : (cup_x_pixel_height - x / VOLUME_X * cup_x_pixel_height).toString() + "px"} , speed / 2);
        $("#cup_y").animate({"padding-top" : (cup_y_pixel_height - y / VOLUME_Y * cup_y_pixel_height).toString() + "px"} , speed / 2 , function(){        console.log("HEIGHT -> " + $("#cup_x").css("height") + " " + $('#cup_y').css("height"));         console.log("padding-top -> " + $("#cup_x").css("padding-top") + " " + $('#cup_y').css("padding-top"));});
        // $("#cup_x_content").height(x / VOLUME_X * $("#cup_x").height());
        // $("#cup_y_content").height(y / VOLUME_Y * $("#cup_y").height());
        document.getElementById("cup_x_description").innerHTML = x.toString();
        document.getElementById("cup_y_description").innerHTML = y.toString();

        setTimeout(function(){
            animate(depth + 1 , array_of_results);
        } , speed);
        return;
    }

    var initializeCupSize = function(volume_x , volume_y){
        //Re-initialize padding and margin
        $(".cups").css({
            "padding-top" : "0px",
            "margin-top" : "0px"
        });
        $(volume_y > volume_x ? "#cup_y" : "#cup_x").height(maximum_pixel_height);
        $(volume_y > volume_x ? "#cup_x" : "#cup_y").height(maximum_pixel_height * (volume_x > volume_y ? (volume_y/volume_x) : volume_x / volume_y));

        console.log("heightx -> " + $("#cup_x").height());
        console.log("heighty -> " + $("#cup_y").height());

        cup_x_pixel_height = $("#cup_x").height();
        cup_y_pixel_height = $("#cup_y").height();

        //$("#cup_x").height(cup_x_pixel_height + 10);
        //$("#cup_y").height(cup_y_pixel_height + 10);

        $((volume_x > volume_y ? "#cup_y" : "#cup_x")).css("margin-top" , Math.abs(cup_y_pixel_height - cup_x_pixel_height));

        // start with zero volume
        $("#cup_x").css("padding-top" , ($("#cup_x").css("height")));
        $("#cup_y").css("padding-top" , ($("#cup_y").css("height")));
        console.log("padding-top " + $("#cup_x").css("padding-top") + " , " + $("#cup_y").css("padding-top") )
    }


    try{
        document.getElementById("trigger_event_button").disabled = true;
        if(VOLUME_X < 0 || VOLUME_Y < 0 || TARGET_VOLUME < 0 || isNaN(VOLUME_X) || isNaN(VOLUME_Y) || isNaN(TARGET_VOLUME)){
            print_notice("Invalid volume of cups entered!" , false , speed);
            document.getElementById("trigger_event_button").disabled = false;
            return;
        }

        initializeCupSize(VOLUME_X , VOLUME_Y);
        var array_of_results = get_transformation_method(VOLUME_X , VOLUME_Y , TARGET_VOLUME);

        if(array_of_results === false){
            print_notice("Cannot find any possible ways to reach that state" , false , speed);
            //alert("No possible ways to reach that target volume!");
        }
        else{

            print_notice("Found a way to reach the target volume in <strong>" + (array_of_results.length - 1).toString() + "</strong> step" + (array_of_results.length -1 == 1 ? "" : "s") + "!" , true , speed);
            animate(0 , array_of_results);

        }
    }
    catch(err){
        document.getElementById("trigger_event_button").disabled = false;
        print_notice(err , false);
        //window.reload();
    }
}

var triggerEventSimulation = function(){
    var x = Number(document.getElementById('input_first_cup_volume').value);
    var y = Number(document.getElementById('input_second_cup_volume').value);
    var z = Number(document.getElementById('input_target_cup_volume').value);
    var speed = Number(document.getElementById("input_target_speed").value);
    if(document.getElementById("input_target_speed").value == "")
        speed = 1000;
    simulateEvent(x , y , z , speed);
}

$(document).ready(function(){
    
});