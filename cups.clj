(let [max-x 9 max-y 4 target-volume 2]
  (defstruct cup-state :x :y)

  (defn is-target-volume? [state]
    (or 
      (= target-volume (get state :x))
      (= target-volume (get state :x))
    )
  )

  (defn get-next-states [state]
    (def x (get state :x))
    (def y (get state :y))
    [
      ; pour x -> y
      (struct cup-state (- x (- (min max-y (+ y x)) y)) (min max-y (+ y x)) )
      ; pour y -> x
      (struct cup-state (min max-x (+ y x)) (- y (- (min max-x (+ y x)) x)) )
      ; fill x
      (struct cup-state max-x y)
      ; fill y
      (struct cup-state x max-y)
      ; empty x
      (struct cup-state 0 y)
      ; empty y
      (struct cup-state x 0)
    ]
  )

  (defn transverse [stack visited]
    (def latest-state (last (first stack)))

    (let [ [accepted new-visited]
      (loop [ states (get-next-states latest-state) v visited arr []]
        (if (empty? states)
          [arr v]
          (do
            (def state (first states))
            (if (contains? v state)
              (recur (next states) v arr)
              (recur (next states) (conj v state) (conj arr state))
            )              
          )
        )
      )]

      [ (concat (next stack) (map #(conj (first stack) %) accepted)) new-visited ]        
    )
  )

  (def root-state (struct cup-state 0 0) )
  (def final (loop [stack (list [ root-state ] ) visited #{ root-state } ]
    (if (empty? stack) 
      false
      (do 
        (if (is-target-volume? (last (first stack) ))
          (first stack)
          (let [ [stack visited] (transverse stack visited) ]
            (recur stack visited)
          )
        )
      )
    )
  ))
  
  (if-not final
    (println "No possible sequence found!")
    (doseq [ entry final ]
      (println (str "(" (entry :x) ", " (entry :y) ")"))
    )
  )
)