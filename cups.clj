(defn make-get-next-states [max-x max-y]
  (fn [state]
    (let [x (:x state) y (:y state)]
      (set [{:x (- x (- (min max-y (+ y x)) y)) :y (min max-y (+ y x))}
            {:x (min max-x (+ x y)) :y (- y (- (min max-x (+ x y)) x))}
            {:x x :y max-y}
            {:x max-x :y y}
            {:x 0 :y y}
            {:x x :y 0}]))))

(defn transverse [root-state get-next-states is-target-volume?]
  (->> (iterate (fn [[arr visited]]
                  (let [next-states (filter #(not (contains? visited %))
                                            (-> arr first last get-next-states))]
                    [(concat (next arr)
                             (map #(conj (first arr) %) next-states))
                     (clojure.set/union visited (set next-states))]))
           [[[root-state]] #{}] )
    (filter #(or (-> % first emtpy?) (-> % first last last is-target-volume?)))
    first first last))

(defn make-is-target-volume? [target-volume]
  (fn [state] (or (= target-volume (:x state)) (= target-volume (:y state)))))

(defn solve [max-x max-y target-volume]
  (transverse {:x 0 :y 0}
              (make-get-next-states max-x max-y)
              (make-is-target-volume? target-volume)))

; NB: unpure methods (interacts with the outside world) follows:
(let [flag? (>= (count *command-line-args*) 4)
      max-x (if flag? (Integer. (nth *command-line-args* 1)) 9)
      max-y (if flag? (Integer. (nth *command-line-args* 2)) 4)
      target (if flag? (Integer. (nth *command-line-args* 3)) 2)
      soln (solve max-x max-y target)]
  (if soln
    (do (println "Found a solution in" (- (count soln) 1) "steps!")
      (doseq [step soln] (println step)))
    (println "Solution not found!")))
