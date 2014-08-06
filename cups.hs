import Data.Maybe
import System.Environment
import qualified Data.Map as Map
import qualified Data.Sequence as Seq

data Cups = Cups {-# UNPACK #-} !Int
                 {-# UNPACK #-} !Int
    deriving (Eq, Ord)

instance Show Cups where
    show (Cups x y) = "(" ++ show x ++ ", " ++ show y ++ ")"

emptyX :: Int -> Int -> Cups -> Cups
emptyX _ _ (Cups _ y) = Cups 0 y

emptyY :: Int -> Int -> Cups -> Cups
emptyY _ _ (Cups x _) = Cups x 0

fillX :: Int -> Int -> Cups -> Cups
fillX volumeX _ (Cups _ y) = Cups volumeX y

fillY :: Int -> Int -> Cups -> Cups
fillY _ volumeY (Cups x _) = Cups x volumeY

pourXY :: Int -> Int -> Cups -> Cups
pourXY _ volumeY (Cups x y) = Cups (x+y-y') y' where y' = min (x+y) volumeY

pourYX :: Int -> Int -> Cups -> Cups
pourYX volumeX _ (Cups x y) = Cups x' (x+y-x') where x' = min (x+y) volumeX

expand :: Ord a => [a -> a] -> (a -> Bool) -> a -> (Maybe a, Map.Map a a)
expand fneighbors fulfillTarget start =
    expand' (Seq.singleton start, Map.singleton start start)
  where
    expand' (queue, parents) =
        case Seq.viewl queue of
            Seq.EmptyL -> (Nothing, parents)
            x Seq.:< xs -> if fulfillTarget x
                then (Just x, parents)
                else expand' (queue', parents')
              where
                (queue', parents') = foldr expand'' (xs, parents) neighbors
                expand'' = (\ v t@(q, p) -> if Map.member v p
                    then t
                    else (q Seq.|> v, Map.insert v x p))
                neighbors = map ($ x) fneighbors

findPath :: Ord a => [a -> a] -> (a -> Bool) -> a -> Maybe [a]
findPath fneighbors fulfillTarget start =
    maybe Nothing (Just . reverse . findPath') maybeDest
  where
    (maybeDest, parents) = expand fneighbors fulfillTarget start
    findPath' x = if x == start
        then [start]
        else x : (findPath' $ fromJust $ Map.lookup x parents)

findPathCups :: Int -> Int -> Int -> Maybe [Cups]
findPathCups volumeX volumeY targetVolume =
    findPath fneighbors fulfillTarget (Cups 0 0)
  where
    fneighbors = map (\ f -> f volumeX volumeY)
        [emptyX, emptyY, fillX, fillY, pourXY, pourYX]
    fulfillTarget (Cups x y) = any (== targetVolume) [x, y]

main :: IO ()
main = do
    args <- fmap (map read) getArgs
    let maybePath = findPathCups
            (case drop 0 args of x:_ -> x; [] -> 4)
            (case drop 1 args of x:_ -> x; [] -> 9)
            (case drop 2 args of x:_ -> x; [] -> 2)
    case maybePath of
        Nothing -> putStrLn "No possible solution!"
        Just path -> do
            let nstep = length path - 1
            putStrLn $ "Found an answer with " ++ show nstep ++ " steps!"
            mapM_ print path

