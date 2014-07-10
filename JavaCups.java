import java.util.ArrayDeque;
import java.util.HashMap;

public class JavaCups {
    public static void main(String[] args) {
        if (args.length >= 3) {
            Cups.VOLUME_X = Integer.parseInt(args[0]);
            Cups.VOLUME_Y = Integer.parseInt(args[1]);
            Cups.TARGET_VOLUME = Integer.parseInt(args[2]);
        } else {
            Cups.VOLUME_X = 4;
            Cups.VOLUME_Y = 9;
            Cups.TARGET_VOLUME = 2;
        }


        ArrayDeque<Cups> queue = new ArrayDeque<Cups>();
        HashMap<Cups, CupsStates> states = new HashMap<Cups, CupsStates>();

        states.put(new Cups(0, 0), new CupsStates(0, null));
        queue.add(new Cups(0, 0));

        while (!queue.isEmpty()) {
            Cups currentCups = queue.poll();
            Cups nextCups;
            CupsStates currentState = states.get(currentCups);
            CupsStates nextState = new CupsStates(currentState.getSteps() + 1, currentCups);

            if (currentCups.valid()) {
                System.out.println(currentState.getSteps());
                ArrayDeque<Cups> outputStack = new ArrayDeque<Cups>();

                while (currentCups != null) {
                    outputStack.push(currentCups);
                    currentCups = states.get(currentCups).getParent();
                }

                while (!outputStack.isEmpty()) {
                    System.out.println(outputStack.pop());
                }
                break;
            }

            nextCups = currentCups.fillX();
            if (states.get(nextCups) == null) {
                states.put(nextCups, nextState);
                queue.add(nextCups);
            }

            nextCups = currentCups.fillY();
            if (states.get(nextCups) == null) {
                states.put(nextCups, nextState);
                queue.add(nextCups);
            }

            nextCups = currentCups.emptyX();
            if (states.get(nextCups) == null) {
                states.put(nextCups, nextState);
                queue.add(nextCups);
            }

            nextCups = currentCups.emptyY();
            if (states.get(nextCups) == null) {
                states.put(nextCups, nextState);
                queue.add(nextCups);
            }

            nextCups = currentCups.pourXY();
            if (states.get(nextCups) == null) {
                states.put(nextCups, nextState);
                queue.add(nextCups);
            }

            nextCups = currentCups.pourYX();
            if (states.get(nextCups) == null) {
                states.put(nextCups, nextState);
                queue.add(nextCups);
            }
        }
    }
}

class Cups {
    public static int VOLUME_X, VOLUME_Y, TARGET_VOLUME;
    private int x, y;

    public Cups(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public int getX() { return x; }
    public int getY() { return y; }

    public boolean valid() {
        return x == TARGET_VOLUME || y == TARGET_VOLUME;
    }

    @Override
    public boolean equals(Object another) {
        if (another instanceof Cups) {
            Cups other = (Cups)another;
            return other.getX() == x && other.getY() == y;
        }
        return false;
    }

    @Override
    public int hashCode() {
        return toString().hashCode();
    }

    @Override
    public String toString() {
        return "(" + x + ", " + y + ")";
    }

    public Cups fillX()  { return new Cups(VOLUME_X, y); }
    public Cups fillY()  { return new Cups(x, VOLUME_Y); }

    public Cups emptyX() { return new Cups(0, y); }
    public Cups emptyY() { return new Cups(x, 0); }

    public Cups pourXY() {
        int newY = Math.min(VOLUME_Y, x + y);
        int newX = x - (newY - y);
        return new Cups(newX, newY);
    }

    public Cups pourYX() {
        int newX = Math.min(VOLUME_X, x + y);
        int newY = y - (newX - x);
        return new Cups(newX, newY);
    }
}

class CupsStates {
    private int steps;
    private Cups parent;

    public CupsStates(int steps, Cups parent) {
        this.steps = steps;
        this.parent = parent;
    }

    public int getSteps() { return steps; }
    public Cups getParent() { return parent; }
}
