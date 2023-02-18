import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.concurrent.ThreadLocalRandom;

import static java.lang.Integer.min;
import static java.lang.Integer.parseInt;


class LoseException extends Exception {}

class Pair<L, R> {
    L l;
    R r;
    Pair(L l, R r) {
        this.l = l;
        this.r = r;
    }

    @Override
    public String toString() {
        return "[" + l.toString() + ", " + r.toString() + "]";
    }
}

/**
 * Container of data returned from any of algorithms
 */
class AlgorithmResult {
    ArrayList<Pair<Integer, Integer>> pathPoints;
    String mapString;
    Double executionDuration;

    AlgorithmResult(ArrayList<Pair<Integer, Integer>> pathPoints, String mapString) {
        this.pathPoints = pathPoints;
        this.mapString = mapString;
    }

    @Override
    public String toString() {
        return String.format("LENGTH: %d\nPATH:\n%s\n%s", pathPoints.size(), mapString, pathPoints);
    }
}

enum GlobalResultType {
    JACK_WINS,
    KRAKEN_WINS,
    LOSE
}

enum AlgorithmType {
    BACKTRACKING,
    A_STAR
}

class Map {
    static int MAX_INT = 1000; // Alternative of infinity, in terms of map dimensions
    int BACKTRACKING_MAX_PATH_LEN = 20; // search for paths shorter than this const
    //    static int BACKTRACKING_KILL_KRAKEN_MAX_PATH = 20;
    static int sizex = 9;
    static int sizey = 9; // Map dimensions

    Cell startCell;
    Cell finishCell; // Cells for whom we are performing current algorithm

    Cell jackCell;
    Cell djCell;
    Cell chestCell;
    Cell tortugaCell;
    Cell krakenCell;
    Cell rockCell;

    int backtrackingMinDist;

    enum CellType{
        JACK,
        EMPTY,
        CHEST,
        TORTUGA,
        DJ,
        KRAKEN,
        ROCK,
        DANGEROUS,
    }

    ArrayList<ArrayList<Cell>> cells = new ArrayList<>();

    /**
     * Class representing map cell, with position (posx, posy)
     * Contains algorithms state related fields and methods: `visited`, `parentCell`, `g`, `h()`,
     */
    static class Cell {
        CellType type;
        int posx;
        int posy; // Position of Cell
        boolean visited;
        int g; // AStar related field
        Cell parentCell = null;

        Cell(CellType type, int posx, int posy) {
            this.type = type;
            this.posx = posx;
            this.posy = posy;
            this.resetState();
        }

        // h: Min possible dist from Cell to finishCell
        public int h(Cell finishCell) {
            int diffX = Math.abs(finishCell.posx - posx);
            int diffY = Math.abs(finishCell.posy - posy);
            return Math.max(diffX, diffY);
        }

        public boolean jackCanMoveOn(boolean tortugaVisited) {
            return this.type != CellType.DANGEROUS &&
                    this.type != CellType.DJ &&
                    (this.type != CellType.KRAKEN || tortugaVisited) &&
                    this.type != CellType.ROCK;
        }

        /**
         * Reset algorithm related fields to initial, to prepare for the next algorithm run
         */
        public void resetState() {
            this.g = MAX_INT;
            this.parentCell = null;
            this.visited = false;
        }

        public boolean isNeighbour(Cell c) {
            return ( Math.abs(c.posx - this.posx) <= 1 && Math.abs(c.posy - this.posy) <= 1 );
        }

        @Override
        public String toString() {
            return this.type.toString();
        }

        /**
         * @return Char representation, used in `Map.cellsToStrings`
         */
        public String toStringMap() {
//            if (visited) { return "V"; }
            if (type == CellType.EMPTY) { return "."; }
            if (type == CellType.JACK) { return "J"; }
            if (type == CellType.CHEST) { return "C"; }
            if (type == CellType.DJ) { return "D"; }
            if (type == CellType.ROCK) { return "R"; }
            if (type == CellType.KRAKEN) { return "K"; }
            if (type == CellType.DANGEROUS) { return "#️"; }
            if (type == CellType.TORTUGA) { return "T"; }
            return "@";
        }

        /**
         * @return One of the arrow chars, representing parent cell direction
         */
        public String toStringParent() {
            if (this.parentCell == null) { return "0"; }
            if (this.parentCell.posx > this.posx && this.parentCell.posy == this.posy) {
                return "⬅";
            }
            if (this.parentCell.posy > this.posy && this.parentCell.posx == this.posx) {
                return "↑";
            }
            if (this.parentCell.posx > this.posx && this.parentCell.posy > this.posy) {
                return "↖";
            }
            if (this.parentCell.posx < this.posx && this.parentCell.posy > this.posy) {
                return "↗️";
            }
            if (this.parentCell.posx > this.posx && this.parentCell.posy < this.posy) {
                return "↙";
            }
            if (this.parentCell.posx == this.posx && this.parentCell.posy < this.posy) {
                return "↓";
            }
            if (this.parentCell.posx < this.posx && this.parentCell.posy < this.posy) {
                return "↘";
            }
            if (this.parentCell.posx < this.posx && this.parentCell.posy == this.posy) {
                return "➜";
            }
            System.out.printf("%d %d <- %d %d\n", this.parentCell.posx, this.parentCell.posy, this.posx, this.posy);
            return "?";
        }
    }

    /**
     * Initial map generation, filling `this.cells` array with "Cell" objects
     */
    public Map(int jx, int jy, int djx, int djy, int kx, int ky, int rx, int ry, int cx, int cy, int tx, int ty) {
        for (int i = 0; i < sizey; i++) {
            cells.add(new ArrayList<>());
            for (int j = 0; j < sizex; j++) {
                Cell newCell = new Cell(CellType.EMPTY, j, i);
                cells.get(i).add(newCell);
            }
        }

        jackCell = cells.get(jy).get(jx);
        jackCell.type = CellType.JACK;

        chestCell = cells.get(cy).get(cx);
        chestCell.type = CellType.CHEST;

        djCell = cells.get(djy).get(djx);
        djCell.type = CellType.DJ;

        for (Cell c : getNeighbours(djCell, true)) {
            c.type = CellType.DANGEROUS;
        }

        krakenCell = cells.get(ky).get(kx);
        krakenCell.type = CellType.KRAKEN;

        for (Cell c : getNeighbours(krakenCell, false)) {
            c.type = CellType.DANGEROUS;
        }

        rockCell = cells.get(ry).get(rx);
        rockCell.type = CellType.ROCK;

        tortugaCell = cells.get(ty).get(tx);
        tortugaCell.type = CellType.TORTUGA;
    }

    @Override
    public String toString() {
        return String.format("[%d,%d] [%d,%d] [%d,%d] [%d,%d] [%d,%d] [%d,%d]\n%s",
                jackCell.posx,
                jackCell.posy,
                djCell.posx,
                djCell.posy,
                krakenCell.posx,
                krakenCell.posy,
                rockCell.posx,
                rockCell.posy,
                chestCell.posx,
                chestCell.posy,
                tortugaCell.posx,
                tortugaCell.posy,
                concatStringArr(cellsToStrings())
        );
    }

    /**
     * Reset cells state & algorithms related variables values to initial, after performing some algorithm
     */
    public void resetCellsState() {
        backtrackingMinDist = BACKTRACKING_MAX_PATH_LEN;
        // Return changed cells state to cells given from initial generation
        for (ArrayList<Cell> cellArr: cells) {
            for (Cell c: cellArr) {
                c.resetState();
            }
        }
    }

    /**
     * @param c Given cell
     * @param isMoore whether we search for 8 neighbours or just for 4
     * @return list of neighbour cells
     */
    public ArrayList<Cell> getNeighbours(Cell c, boolean isMoore) {
        int i = c.posy;
        int j = c.posx;

        ArrayList<Cell> neigh = new ArrayList<>();

        if (i > 0) {
            neigh.add(cells.get(i - 1).get(j));
        }
        if (j > 0) {
            neigh.add(cells.get(i).get(j - 1));
        }
        if (i < sizey - 1) {
            neigh.add(cells.get(i + 1).get(j));
        }
        if (j < sizex - 1) {
            neigh.add(cells.get(i).get(j + 1));
        }
        if (isMoore) {
            if (i > 0 && j > 0) {
                neigh.add(cells.get(i - 1).get(j - 1));
            }
            if (i < sizey - 1 && j < sizex - 1) {
                neigh.add(cells.get(i + 1).get(j + 1));
            }
            if (i > 0 && j < sizex - 1) {
                neigh.add(cells.get(i - 1).get(j + 1));
            }
            if (i < sizey - 1 && j > 0) {
                neigh.add(cells.get(i + 1).get(j - 1));
            }
        }
        return neigh;
    }

    /**
     * @param isTortugaVisited Whether we count Kraken cell or not
     * @return cell with min (g + h) sum
     */
    public Cell aStarSelectBestCell(boolean isTortugaVisited) {
        int minDistSum = MAX_INT;
        Cell bestCell = null;
        for (int i = 0; i < sizey; i++) {
            for (int j = 0; j < sizex; j++) {
                Cell c = cells.get(i).get(j);
                if ((c.g + c.h(finishCell)) < minDistSum) {
                    if (!c.visited && c.jackCanMoveOn(isTortugaVisited)) {
                        minDistSum = c.g + c.h(finishCell);
                        bestCell = c;
                    }
                }
            }
        }
        return bestCell;
    }

    // Full AStar algo
    public ArrayList<Cell> aStarAlgo(Cell from, Cell to, boolean isTortugaVisited) throws LoseException {
        if (from == to) { return new ArrayList<>(); }

        startCell = from;
        startCell.g = 0;
        finishCell = to;

        while (true) {
            Cell curCell = aStarSelectBestCell(isTortugaVisited);
            if (curCell == null) { break; }
            curCell.visited = true;

            ArrayList<Cell> neigh = getNeighbours(curCell, true);
            for (Cell n : neigh) {
                if (!n.visited && n.jackCanMoveOn(isTortugaVisited)) {
                    int newG = curCell.g + 1;
                    if (newG < n.g) {
                        n.g = newG;
                        n.parentCell = curCell;
                    }
                }
            }
        }

        return retracePath();
    }

    public AlgorithmResult algoByJackMethod(Method algoFunc) throws Exception {
        ArrayList<ArrayList<String>> stringArr = cellsToStrings();

        ArrayList<Cell> path = (ArrayList<Cell>) algoFunc.invoke(this, jackCell, chestCell, false);
        stringArr = addPathToStringArr(stringArr, path);
        resetCellsState();

        return getAlgorithmResult(path, stringArr);
    }

    public AlgorithmResult algoByKrakenMethod(Method algoFunc) throws Exception {
        ArrayList<ArrayList<String>> stringArr = cellsToStrings();

        ArrayList<Cell> path1 = (ArrayList<Cell>) algoFunc.invoke(this, jackCell, tortugaCell, false);
        stringArr = addPathToStringArr(stringArr, path1);
        resetCellsState();

        ArrayList<Cell> path2 = (ArrayList<Cell>) algoFunc.invoke(this,tortugaCell, krakenCell, true);
        stringArr = addPathToStringArr(stringArr, path2);
        resetCellsState();

        ArrayList<Cell> path3 = (ArrayList<Cell>) algoFunc.invoke(this,krakenCell, chestCell, true);
        stringArr = addPathToStringArr(stringArr, path3);
        resetCellsState();

        ArrayList<Cell> resultPath = new ArrayList<>();
        resultPath.addAll(path1);
        resultPath.addAll(path2);
        resultPath.addAll(path3);

        return getAlgorithmResult(resultPath, stringArr);
    }

    // Retrace path from finishCell to startCell, using parents
    public ArrayList<Cell> retracePath() throws LoseException {
        if (finishCell.parentCell == null) { throw new LoseException(); }

        ArrayList<Cell> pathList = new ArrayList<>();

        Cell curCell = finishCell;
        while (curCell.parentCell != null) {
            pathList.add(curCell);
            curCell = curCell.parentCell;
        }
        Collections.reverse(pathList);
        return pathList;
    }

    /**
     * Full backtracking algorithm
     * @param from Start cell
     * @param to Finish cell
     * @param isTortugaVisited Whether we can move on Kraken cell or not
     * @return min path between 2 cells
     * @throws LoseException
     */
    public ArrayList<Cell> backtrackingAlgo(Cell from, Cell to, boolean isTortugaVisited) throws LoseException {
        if (from == to) { return new ArrayList<>(); }

        startCell = from;
        finishCell = to;

        resetCellsState();
        backtrackingIteration(0, startCell, isTortugaVisited);
        return retracePath();
    }

    public AlgorithmResult backtrackingJack() throws Exception {
        Method backtrackingFunc = Map.class.getMethod("backtrackingAlgo", Cell.class, Cell.class, boolean.class);
        return algoByJackMethod(backtrackingFunc);
    }

    public AlgorithmResult aStarJack() throws Exception {
        Method aStarFunc = Map.class.getMethod("aStarAlgo", Cell.class, Cell.class, boolean.class);
        return algoByJackMethod(aStarFunc);
    }

    public AlgorithmResult backtrackingKraken() throws Exception {
        Method backtrackingFunc = Map.class.getMethod("backtrackingAlgo", Cell.class, Cell.class, boolean.class);
        return algoByKrakenMethod(backtrackingFunc);
    }

    public AlgorithmResult aStarKraken() throws Exception {
        Method aStarFunc = Map.class.getMethod("aStarAlgo", Cell.class, Cell.class, boolean.class);
        return algoByKrakenMethod(aStarFunc);
    }

    /**
     * Some kind of bridge to prepare algorithm results as "AlgorithmResult" object as container of data
     * @param path shortest path returned from algorithm
     * @param stringArr array of string symbols representing map, containing full path
     */
    private AlgorithmResult getAlgorithmResult(ArrayList<Cell> path, ArrayList<ArrayList<String>> stringArr) {
        String resultMapString = concatStringArr(stringArr);

        ArrayList<Pair<Integer, Integer>> resultPathInts = new ArrayList<>();
        path.add(0, jackCell);
        for (Cell c : path) {
            resultPathInts.add(new Pair<>(c.posx, c.posy));
        }
        return new AlgorithmResult(resultPathInts, resultMapString);
    }

    public boolean visitedNeighboursCountHeuristic(ArrayList<Cell> neighbours) {
        int visitedCount = 0;
        for (Cell n: neighbours) {
            if (n.visited) { visitedCount++; }
        }
        return visitedCount >= 2;
    }

    /**
     * Main part of backtracking recursive algorithm
     * @param depth of recursion
     * @param curCell Current cell we are working with
     * @param isTortugaVisited whether we can move on Kraken cell
     * @return min path length
     */
    public int backtrackingIteration(int depth, Cell curCell, boolean isTortugaVisited) {
        curCell.visited = true;

        if (curCell == finishCell) {
            backtrackingMinDist = min(backtrackingMinDist, depth);
            curCell.visited = false;
            return depth;
        }

        ArrayList<Cell> neighbours = getNeighbours(curCell, true);
        int neighMinDist = MAX_INT;
        Cell bestNeigh = null;

        for (Cell neigh : neighbours) {
            if (visitedNeighboursCountHeuristic(neighbours)) { break; }
            if (depth + curCell.h(finishCell) >= min(backtrackingMinDist, neighMinDist)) { break; }
            if (!neigh.visited && neigh.jackCanMoveOn(isTortugaVisited)) {
                int neighDist = backtrackingIteration(depth + 1, neigh, isTortugaVisited);
                if (neighDist < neighMinDist) {
                    neighMinDist = neighDist;
                    bestNeigh = neigh;
                }
            }
        }
        if (bestNeigh != null) {
            bestNeigh.parentCell = curCell;
        }
        curCell.visited = false;
        return neighMinDist;
    }


    /**
     * @return array of symbols, each representing map cell
     */
    public ArrayList<ArrayList<String>> cellsToStrings() {
        ArrayList<ArrayList<String>> stringArr = new ArrayList<>();
        for (int i = 0; i < sizey; i++) {
            ArrayList<String> si = new ArrayList<>(sizey);
            for (Cell object : cells.get(i)) {
                si.add(object.toStringMap());
            }
            stringArr.add(si);
        }

        return stringArr;
    }

    /**
     * @param stringArr existing array of chars
     * @param path list of cells
     * @return Get new symbols array by replacing path cells symbols with arrow chars
     */
    public ArrayList<ArrayList<String>> addPathToStringArr(ArrayList<ArrayList<String>> stringArr, ArrayList<Cell> path) {
        if (path != null) {
            for (Cell c : path) {
                String existingChar = stringArr.get(c.posy).get(c.posx);
//                if (Objects.equals(existingChar, ".")) {
                stringArr.get(c.posy).set(c.posx, c.toStringParent());
//                }
            }
        }
        return stringArr;
    }

    /**
     * @param stringArr 2-d array of chars representing cells
     * @return single String for console printing
     */
    public String concatStringArr(ArrayList<ArrayList<String>> stringArr) {
        ArrayList<String> strings = new ArrayList<>();
        for (ArrayList<String> arrc: stringArr) {
            strings.add(String.join(" ", arrc));
        }
        return String.join("\n", strings);
    }

    /**
     * Prints map string representation to console
     */
    public void printMap() {
        ArrayList<ArrayList<String>> stringArr = cellsToStrings();
        String res = concatStringArr(stringArr);
        System.out.println(res);
        System.out.println("\n");
    }

    // Debug func
    public void printParents() {
        ArrayList<ArrayList<String>> stringArr = cellsToStrings();
        for (ArrayList<Cell> arrc : cells) {
            for (Cell c: arrc) {
                String existingChar = stringArr.get(c.posy).get(c.posx);
//                if (Objects.equals(existingChar, ".")) {
                stringArr.get(c.posy).set(c.posx, c.toStringParent());
//                }
            }
        }

        String res = concatStringArr(stringArr);
        System.out.println(res);
    }
}


class Main {
    /**
     * Common func for both backtracking and AStar, to determine in which method (Jack or Kraken) path will be shorter.
     * Prints result to console
     * @param mapObj Map object
     * @param jackAlgo algo func for "Jack" method
     * @param krakenAlgo algo func for "Kraken" method
     */
    public static AlgorithmResult doAlgoWithBothJackAndKrakenMethods(Map mapObj, Method jackAlgo, Method krakenAlgo, AlgorithmType algoType, boolean isDebug) {
        AlgorithmResult resultJack = null, resultKraken = null;
        GlobalResultType resultType;

        long startTime = System.nanoTime();

        try {
            resultJack = (AlgorithmResult) jackAlgo.invoke(mapObj);
            if (isDebug) {
                System.out.println("JACK METHOD - WIN");
                System.out.println(resultJack);
            }

            if (algoType == AlgorithmType.BACKTRACKING) {
                mapObj.BACKTRACKING_MAX_PATH_LEN = resultJack.pathPoints.size();
            }
        } catch (Exception e) {
            if (isDebug) { System.out.println("JACK METHOD - LOSE"); }
        }
        try {
            resultKraken = (AlgorithmResult) krakenAlgo.invoke(mapObj);
            if (isDebug) {
                System.out.println("KRAKEN METHOD - WIN");
                System.out.println(resultKraken);
            }
        } catch (Exception e) {
            if (isDebug) { System.out.println("KRAKEN METHOD - LOSE"); }
        }

        long endTime = System.nanoTime();
        double durationSeconds = (endTime - startTime) / Math.pow(10, 9);

        if (resultKraken != null && resultJack != null) {
            if (resultJack.pathPoints.size() < resultKraken.pathPoints.size()) {
                resultType = GlobalResultType.JACK_WINS;
            } else {
                resultType = GlobalResultType.KRAKEN_WINS;
            }
        } else if (resultKraken != null) {
            resultType = GlobalResultType.KRAKEN_WINS;
        } else if (resultJack != null) {
            resultType = GlobalResultType.JACK_WINS;
        } else {
            resultType = GlobalResultType.LOSE;
        }

        AlgorithmResult returnResult;

        if (resultType != GlobalResultType.LOSE) {
            if (isDebug) { System.out.println("BEST SOLUTION:"); }
            if (resultType == GlobalResultType.JACK_WINS) {
                if (isDebug) { System.out.println(resultJack); }
                returnResult = resultJack;
            } else {
                if (isDebug) { System.out.println(resultKraken); }
                returnResult = resultKraken;
            }
        } else {
            if (isDebug) { System.out.println("OVERALL LOSE :("); }
            return null;
        }
        returnResult.executionDuration = durationSeconds;
        return returnResult;
    }

    /**
     * Highest wrapper function for "A*" algorithm, to get the shortest path
     * @throws NoSuchMethodException
     */
    public static AlgorithmResult findSolutionAStar(Map mapObj, boolean isDebug) throws NoSuchMethodException {
        Method aStarKraken = Map.class.getMethod("aStarKraken");
        Method aStarJack = Map.class.getMethod("aStarJack");

        if (isDebug) { System.out.println("\n--- A STAR METHOD ---"); }
        return doAlgoWithBothJackAndKrakenMethods(mapObj, aStarJack, aStarKraken, AlgorithmType.A_STAR, isDebug);
    }

    /**
     * Highest wrapper function for "Backtracking" algorithm, to get the shortest path
     * @throws NoSuchMethodException
     */
    public static AlgorithmResult findSolutionBacktracking(Map mapObj, boolean isDebug) throws NoSuchMethodException {
        Method backtrackingJack = Map.class.getMethod("backtrackingJack");
        Method backtrackingKraken = Map.class.getMethod("backtrackingKraken");

        if (isDebug) { System.out.println("\n--- BACKTRACKING METHOD ---"); }
        return doAlgoWithBothJackAndKrakenMethods(mapObj, backtrackingJack, backtrackingKraken, AlgorithmType.BACKTRACKING, isDebug);
    }

    public static void main(String arg[]) throws IOException, NoSuchMethodException {
        solveFromFile();
//        statisticalAnalysis();
    }

    public static void solveFromFile() throws IOException, NoSuchMethodException {
        String firstLine = Files.readString(Paths.get("src/input.txt"), StandardCharsets.UTF_8).split("\r\n")[0];
        System.out.println(firstLine);

        ArrayList<Integer> nums = new ArrayList<>();

        String[] splitted = firstLine.split(" ");
        for (String s : splitted) {
            String striped = s.substring(1, s.length() - 1);
            String[] strNums = striped.split(",");
            int n1 = parseInt(strNums[0]);
            int n2 = parseInt(strNums[1]);
            nums.add(n1);
            nums.add(n2);
        }

        int jx = nums.get(0), jy = nums.get(1);
        int djx = nums.get(2), djy = nums.get(3);
        int kx = nums.get(4), ky = nums.get(5);
        int rx = nums.get(6), ry = nums.get(7);
        int cx = nums.get(8), cy = nums.get(9);
        int tx = nums.get(10), ty = nums.get(11);

        Map mapObj = new Map(jx, jy, djx, djy, kx, ky, rx, ry, cx, cy, tx, ty);
        System.out.println("INITIAL MAP");
        mapObj.printMap();

        AlgorithmResult aStarResult = findSolutionAStar(mapObj, true);
        AlgorithmResult backtrackingResult = findSolutionBacktracking(mapObj, true);

        BufferedWriter writer = new BufferedWriter(new FileWriter("outputAStar.txt"));
        if (aStarResult != null) {
            writer.write(aStarResult.toString());
        } else {
            writer.write("Lose");
        }

        writer.close();

        writer = new BufferedWriter(new FileWriter("outputBacktracking.txt"));
        if (aStarResult != null) {
            writer.write(backtrackingResult.toString());
        } else {
            writer.write("Lose");
        }
        writer.close();
    }

    public static int randomNum() {
        return ThreadLocalRandom.current().nextInt(0, 9);
    }

    public static Map generateRandomMap() {
//        int jx = randomNum(), jy = randomNum();
        int jx = 0, jy = 0;
        int djx = randomNum(), djy = randomNum();
        int kx = randomNum(), ky = randomNum();
        int rx = randomNum(), ry = randomNum();
        int cx = randomNum(), cy = randomNum();
        int tx = randomNum(), ty = randomNum();

        return new Map(jx, jy, djx, djy, kx, ky, rx, ry, cx, cy, tx, ty);
    }

    public static boolean isValidMap(Map map) {
        ArrayList<Map.Cell> dangerCells = new ArrayList<>();
        dangerCells.addAll(map.getNeighbours(map.krakenCell, false));
        dangerCells.addAll(map.getNeighbours(map.djCell, true));
        dangerCells.add(map.krakenCell);
        dangerCells.add(map.djCell);

        ArrayList<Map.Cell> tortugaForbiddenCells = new ArrayList<>();
        tortugaForbiddenCells.addAll(dangerCells);
        tortugaForbiddenCells.add(map.chestCell);

        if (tortugaForbiddenCells.contains(map.tortugaCell)) { return false; }

        ArrayList<Map.Cell> chestForbiddenCells = new ArrayList<>();
        chestForbiddenCells.addAll(dangerCells);
        chestForbiddenCells.add(map.jackCell);

        if (chestForbiddenCells.contains(map.chestCell)) { return false; }

        ArrayList<Map.Cell> rockForbiddenCells = new ArrayList<>();
        rockForbiddenCells.add(map.tortugaCell);
        rockForbiddenCells.add(map.chestCell);
        rockForbiddenCells.add(map.djCell);
        rockForbiddenCells.add(map.jackCell);

        if (rockForbiddenCells.contains(map.rockCell)) { return false; }

        ArrayList<Map.Cell> krakenForbiddenCells = new ArrayList<>();
        krakenForbiddenCells.add(map.tortugaCell);
        krakenForbiddenCells.add(map.chestCell);
        krakenForbiddenCells.add(map.djCell);
        krakenForbiddenCells.add(map.jackCell);

        if (krakenForbiddenCells.contains(map.krakenCell)) { return false; }

        ArrayList<Map.Cell> djForbiddenCells = new ArrayList<>();
        djForbiddenCells.add(map.tortugaCell);
        djForbiddenCells.add(map.chestCell);
        djForbiddenCells.add(map.krakenCell);
        djForbiddenCells.add(map.rockCell);
        djForbiddenCells.add(map.jackCell);

        if (djForbiddenCells.contains(map.djCell)) { return false; }

        ArrayList<Map.Cell> jackForbiddenCells = new ArrayList<>();
        jackForbiddenCells.addAll(dangerCells);
        jackForbiddenCells.add(map.chestCell);
        jackForbiddenCells.add(map.krakenCell);
        jackForbiddenCells.add(map.rockCell);

        if (jackForbiddenCells.contains(map.jackCell)) { return false; }

        return true;
    }

    public static void statisticalAnalysis() throws NoSuchMethodException {
        boolean isDebug = false;
        int COUNT = 1000;

        int loseCount = 0;
        int winCount = 0;

        ArrayList<Double> durationAStarList = new ArrayList<>();
        ArrayList<Double> durationBacktrackingList = new ArrayList<>();

        for (int i = 0;i < COUNT;i++) {
            Map genMap = generateRandomMap();
            while (!isValidMap(genMap)) {
                genMap = generateRandomMap();
            }

            if (isDebug) { genMap.printMap(); }

            AlgorithmResult astarSol = findSolutionAStar(genMap, isDebug);
            AlgorithmResult btSol =  findSolutionBacktracking(genMap, isDebug);

            if (astarSol == null && btSol == null) {
                loseCount++;
                System.out.println(genMap);
                continue;
            }
            if ((astarSol != null & btSol == null) || (astarSol == null && btSol != null)) {
                System.out.println("ERR: " + genMap);
                continue;
            }

            durationAStarList.add(astarSol.executionDuration);
            durationBacktrackingList.add(btSol.executionDuration);

            winCount++;
        }


        float winRate = (float) (COUNT - loseCount) / COUNT * 100;

        durationAStarList.sort((Comparator.naturalOrder()));
        durationBacktrackingList.sort(Comparator.naturalOrder());

        double meanAStar = getMean(durationAStarList);
        double meanBacktracking = getMean(durationBacktrackingList);

        double medianAStar = getMedian(durationAStarList);
        double medianBacktracking = getMedian(durationBacktrackingList);

        double sdAStar = getStandardDeviation(durationAStarList);
        double sdBacktracking = getStandardDeviation(durationBacktrackingList);

        System.out.printf("Avg time:\nA*:\t%f\nBT:\t%f\n", meanAStar, meanBacktracking);
        System.out.printf("\nWin count: %d\n", winCount);
        System.out.printf("Lose cout: %d\n", loseCount);
        System.out.printf("Win rate: %f%%\n", winRate);
        System.out.printf("\nMedian:\nA*:\t%f\nBT:\t%f\n", medianAStar, medianBacktracking);
        System.out.printf("\nStandard deviation:\nA*:\t%f\nBT:\t%f\n", sdAStar, sdBacktracking);
    }

    public static double getMean(ArrayList<Double> list) {
        return list.stream().mapToDouble(a -> a).sum() / list.size();
    }

    public static double getMedian(ArrayList<Double> list) {
        if (list.size() % 2 == 0) {
            int i1 = (list.size() - 1) / 2;
            int i2 = i1++;
            return (list.get(i1) + list.get(i2)) / 2;
        } else {
            int i = (list.size() - 1) / 2;
            return list.get(i);
        }
    }

    public static double getStandardDeviation(ArrayList<Double> list) {
        double mean = getMean(list);
        double variance = 0;
        for (double d: list) {
            variance += Math.pow(d - mean, 2);
        }
        variance /= list.size() - 1;
        return Math.pow(variance, 0.5);
    }
}