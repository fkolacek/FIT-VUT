/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: Desk.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida Desk reprezentuje sachovnici
 */

package queen.basis;

import gui.basis.GameDifficulty;
import queen.figures.*;
import java.awt.*;
import java.util.Random;
import java.util.Vector;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class Desk {

    private int dimension;
    private Field battleField[];

    private Color roundColor;
    private Vector rounds;

    /**
     * Konstruktor pro objekt Desk
     * @param dimension rozmery hraciho planu
     */
    public Desk(int dimension){
        this.dimension = dimension;
        this.roundColor = Color.WHITE;
        this.rounds = new Vector();

        this.prepareBattleGround();
        this.prepareSoldiers();
    }

    /**
     * Metoda pro pripravu hraciho planu
     */
    private void prepareBattleGround(){
        this.battleField = new Field[this.dimension * this.dimension];

        Color color = Color.BLACK;

        for(char column = 'a'; column < (char)('a' + this.dimension); column++){
            for(int row = 1; row <= this.dimension; row++){
                this.battleField[this.pos(column, row)] = new Field(new Position(column, row), color);

                color = (color == Color.WHITE)? Color.BLACK : Color.WHITE;
            }

            color = (color == Color.WHITE)? Color.BLACK : Color.WHITE;
        }
    }

    /**
     * Metoda pro pripravu pescu
     */
    private void prepareSoldiers(){
        this.at('a', 7).setFigure(new Stone(this, new Position('a', 7), Color.BLACK));
        this.at('b', 6).setFigure(new Stone(this, new Position('b', 6), Color.BLACK));
        this.at('b', 8).setFigure(new Stone(this, new Position('b', 8), Color.BLACK));
        this.at('c', 7).setFigure(new Stone(this, new Position('c', 7), Color.BLACK));
        this.at('d', 6).setFigure(new Stone(this, new Position('d', 6), Color.BLACK));
        this.at('d', 8).setFigure(new Stone(this, new Position('d', 8), Color.BLACK));
        this.at('e', 7).setFigure(new Stone(this, new Position('e', 7), Color.BLACK));
        this.at('f', 6).setFigure(new Stone(this, new Position('f', 6), Color.BLACK));
        this.at('f', 8).setFigure(new Stone(this, new Position('f', 8), Color.BLACK));
        this.at('g', 7).setFigure(new Stone(this, new Position('g', 7), Color.BLACK));
        this.at('h', 6).setFigure(new Stone(this, new Position('h', 6), Color.BLACK));
        this.at('h', 8).setFigure(new Stone(this, new Position('h', 8), Color.BLACK));

        this.at('a', 1).setFigure(new Stone(this, new Position('a', 1), Color.WHITE));
        this.at('a', 3).setFigure(new Stone(this, new Position('a', 3), Color.WHITE));
        this.at('b', 2).setFigure(new Stone(this, new Position('b', 2), Color.WHITE));
        this.at('c', 1).setFigure(new Stone(this, new Position('c', 1), Color.WHITE));
        this.at('c', 3).setFigure(new Stone(this, new Position('c', 3), Color.WHITE));
        this.at('d', 2).setFigure(new Stone(this, new Position('d', 2), Color.WHITE));
        this.at('e', 1).setFigure(new Stone(this, new Position('e', 1), Color.WHITE));
        this.at('e', 3).setFigure(new Stone(this, new Position('e', 3), Color.WHITE));
        this.at('f', 2).setFigure(new Stone(this, new Position('f', 2), Color.WHITE));
        this.at('g', 1).setFigure(new Stone(this, new Position('g', 1), Color.WHITE));
        this.at('g', 3).setFigure(new Stone(this, new Position('g', 3), Color.WHITE));
        this.at('h', 2).setFigure(new Stone(this, new Position('h', 2), Color.WHITE));
    }

    /**
     * Zjisteni, jestli je tato pozice v hracim planu
     * @param column znak sloupce
     * @param row cislo radku
     * @return jsme nebo nejsme na hracim planu
     */
    public boolean isDeserter(char column, int row){
        return this.isDeserter(new Position(column, row));
    }

    /**
     * Zjisteni, jestli je tato pozice v hracim planu
     * @param position pozice
     * @return jsme nebo nejsme na hracim planu
     */
    public boolean isDeserter(Position position){

        if(position.getColumn() < 'a' || position.getColumn() >= 'a' + this.dimension)
            return true;

        if(position.getRow() < 1 || position.getRow() > this.dimension)
            return true;

        return false;
    }

    /**
     * Vraci odkaz na pole na dane pozici
     * @param column znak sloupce
     * @param row cislo radku
     * @return odkaz na pole
     */
    public Field at(char column, int row){
        return this.at(new Position(column, row));
    }

    /**
     * Vraci odkaz na pole na dane pozici
     * @param position pozice
     * @return odkaz na pole
     */
    public Field at(Position position){
        return this.battleField[this.pos(position)];
    }

    /**
     * Vraci index dane pozice na hernim planu
     * @param column znak sloupce
     * @param row cislo radku
     * @return index pozice
     */
    public int pos(char column, int row){
        return this.pos(new Position(column, row));
    }

    /**
     * Vraci index dane pozice na hernim planu
     * @param position pozice
     * @return index pozice
     */
    public int pos(Position position){
        int column = Character.toLowerCase(position.getColumn());

        column -= 'a';

        return ((position.getRow() - 1) * this.dimension) + column;
    }

    /**
     * Skoci s figurkou z mista na misto, je-li to mozne
     * @param fromColumn znak zdrojoveho sloupce
     * @param fromRow cislo zdrojoveho radku
     * @param toColumn znak ciloveho sloupce
     * @param toRow cislo ciloveho radku
     * @return bylo pohnuto figurkou
     */
    public Vector move(char fromColumn, int fromRow, char toColumn, int toRow){
        return this.move(new Position(fromColumn, fromRow), new Position(fromColumn, fromRow));
    }

    /**
     * Skoci s figurkou z mista na misto, je-li to mozne
     * @param from zdrojova pozice
     * @param to cilova pozice
     * @return bylo pohnuto figurkou
     */
    public Vector move(Position from, Position to) throws RuntimeException{

        /* PRESUNOUT!
        // Nelze skakat mimo sachovnici
        if(this.isDeserter(from) || this.isDeserter(to))
            return false;

        // Skok na stejnou pozici
        if(from.equals(to))
            return false;

        // Nelze skocit na jinou figurku
        if(this.at(to).getFigure() != null)
            return false;
        */

        Figure figure = this.at(from).getFigure();

        // Pokud neni cim skakat
        if(figure == null)
            throw new RuntimeException("Tah lze provést pouze s figurkou!");

        if(figure.getColor() != this.roundColor)
            throw new RuntimeException("Hráč není na tahu!");

        Vector assassins = this.getReadyAssassins();
        if(assassins.size() > 0){
            boolean isAssassin = false;

            for(int i = 0; i < assassins.size(); i++){
                Position assassin = (Position)assassins.get(i);

                if(assassin.equals(from)){
                    isAssassin = true;
                    break;
                }
            }

            if(!isAssassin)
                throw new RuntimeException("Hráč je povinen provést skok!");
        }

        // Figurka se tam neumi dostat
        if(!figure.canMove(to))
            throw new RuntimeException("Nelze provést tah na toto pole!");

        Vector victims = figure.move(to);

        for(int i = 0; i < victims.size(); i++){
            Position position = (Position) victims.get(i);

            this.at(position).removeFigure();
        }

        this.rounds.add(new Move(from, to, (victims.size() == 0)? false : true));

        if(figure.getColor() == Color.WHITE && to.getRow() == this.dimension && !"Rook".equals(figure.getClass().getSimpleName()))
            figure = new Rook(this, to, Color.WHITE);

        if(figure.getColor() == Color.BLACK && to.getRow() == 1 && !"Rook".equals(figure.getClass().getSimpleName()))
            figure = new Rook(this, to, Color.BLACK);

        this.at(from).removeFigure();
        this.at(to).setFigure(figure);

        this.roundColor = (this.roundColor == Color.WHITE)? Color.BLACK : Color.WHITE;

        return victims;
    }

    /**
     * Testovani konce hry
     * @return je konec hry?
     */
    public boolean isEndOfAllHope(){

        for(int i = 0; i < this.battleField.length; i++){
            if(this.battleField[i].getFigure() == null)
                continue;

            if(this.battleField[i].getFigure().getColor() != this.roundColor)
                continue;

            if(this.battleField[i].getFigure().canMovePossibilities().size() > 0)
                return false;
        }

        return true;
    }

    /**
     * Vraci seznam figurek pripravenych ke skoku
     * @param roundColor barva hrace, ktery je na tahu
     * @return seznam figurek pripravenych ke skoku
     */
    public Vector getReadyAssassins(Color roundColor){

        Color tmp = this.roundColor;

        this.roundColor = roundColor;

        Vector result = this.getReadyAssassins();

        this.roundColor = tmp;

        return result;
    }

    /**
     * Vraci vsechny figurky, ktere musi skakat
     * @return Vector figurek, ktere musi skocit
     */
    public Vector getReadyAssassins(){
        Vector assassins = new Vector();

        for(int i = 0; i < this.battleField.length; i++){
            if(this.battleField[i].getFigure() == null)
                continue;

            if(this.battleField[i].getFigure().getColor() != this.roundColor)
                continue;

            Vector possibilities = this.battleField[i].getFigure().canMovePossibilities();

            if(possibilities.size() > 0){
                for(int j = 0; j < possibilities.size(); j++){
                    Possibility possibility = (Possibility)possibilities.get(j);

                    if(possibility.killed() > 0)
                        assassins.add(new Position(possibility.getKiller()));
                }
            }
        }

        return assassins;
    }

    /**
     * Getter barvy hrace, ktery prave hraje
     * @return barva aktivniho hrace
     */
    public Color getRoundColor(){
        return this.roundColor;
    }

    /**
     * Getter odehranych kol
     * @return vektor odehranych kol
     */
    public Vector getRounds(){
        return this.rounds;
    }

    public Vector minimax(Vector possible, GameDifficulty gameDifficulty, int depth){
        Vector best = new Vector();
        int best_move_old = -1000, best_move_new = 0;

        for (int i = 0; i < possible.size(); i++){
            Possibility possibility = (Possibility)possible.get(i);
            Desk tmp = new Desk(this.dimension);
            for (int step = 0; step < this.rounds.size(); step++){
                tmp.move(((Move)this.rounds.get(step)).getFrom(), ((Move)this.rounds.get(step)).getTo());
            }
            tmp.move(possibility.getKiller(), possibility.getPosition());
            Vector assassins = tmp.getReadyAssassins();

            for (int j = 0; j < assassins.size(); j++){
                Vector pos_moves = tmp.at((Position)assassins.get(j)).getFigure().canMovePossibilities();
                for (int k = 0; k < pos_moves.size(); k++) {
                    Vector victims = ((Possibility)pos_moves.get(k)).getVictims();
                    for (int l = 0; l < victims.size(); l++){
                        if (((Position)possibility.getPosition()).equals((Position)victims.get(l)))
                            best_move_new--; // odecteme 1 za kazdou figurku, ktera me potom bude ohrozovat
                    }
                }
            }
            if (gameDifficulty == GameDifficulty.GAME_CHUCK_NORRIS && depth > 0 && !tmp.isEndOfAllHope()) { // opravdu tezsi AI
                Vector tmp_assa = tmp.getReadyAssassins();
                Vector tmp_poss = new Vector();
                if (tmp_assa.size() > 0)
                    for(int j = 0; j < tmp_assa.size(); j++){
                        Position act_assa = (Position)tmp_assa.get(j);

                        Vector act_poss = tmp.at(act_assa).getFigure().canMovePossibilities();

                        if(act_poss.size() > 0)
                            for(int k = 0; k < act_poss.size(); k++)
                                tmp_poss.add(act_poss.get(k));
                    }
                else
                    for(int j = 0; j < tmp.battleField.length; j++){
                        if(tmp.battleField[j].getFigure() == null)
                            continue;

                        if(tmp.battleField[j].getFigure().getColor() != tmp.roundColor)
                            continue;

                        Vector act_poss = tmp.battleField[j].getFigure().canMovePossibilities();

                        if(act_poss.size() > 0){
                            for(int k = 0; k < act_poss.size(); k++){
                                tmp_poss.add(act_poss.get(k));
                            }
                        }
                    }
                Vector tmp_result = tmp.minimax(tmp_poss, gameDifficulty, --depth); // rekurze
                Possibility next_move = (Possibility)tmp_result.get(0);
                best_move_new -= (int)tmp_result.get(1)*(depth/4);

                if (next_move.getKiller().getColumn() == 'a' || next_move.getKiller().getColumn() == 'h' ||
                        next_move.getKiller().getRow() == 1 || next_move.getKiller().getRow() == this.dimension)
                    best_move_new++;
                if (next_move.getPosition().getColumn() == 'a' || next_move.getPosition().getColumn() == 'h' ||
                        next_move.getPosition().getRow() == 1 || next_move.getPosition().getRow() == this.dimension)
                    best_move_new--;
                if (tmp.at(next_move.getKiller()).getFigure().getClass().getSimpleName() == "Stone" && (
                        (tmp.at(next_move.getKiller()).getFigure().getColor() == Color.WHITE && next_move.getPosition().getRow() == this.dimension) ||
                        (tmp.at(next_move.getKiller()).getFigure().getColor() == Color.BLACK && next_move.getPosition().getRow() == 1)))
                    best_move_new--;

                tmp.move(next_move.getKiller(), next_move.getPosition());

                assassins = tmp.getReadyAssassins();
                for (int j = 0; j < assassins.size(); j++){
                    Vector pos_moves = tmp.at((Position)assassins.get(j)).getFigure().canMovePossibilities();
                    for (int k = 0; k < pos_moves.size(); k++) {
                        Vector victims = ((Possibility)pos_moves.get(k)).getVictims();
                        for (int l = 0; l < victims.size(); l++){
                            if (((Position)next_move.getPosition()).equals((Position)victims.get(l))) {
                                best_move_new++; // pricteme 1 za kazdou figurku, kterou budeme ohrozovat
                                if (tmp.at(next_move.getPosition()).getFigure().getClass().getSimpleName() == "Rook")
                                    best_move_new++;
                            }
                        }
                    }
                }
            } // konec chucka norrise
            best_move_new += possibility.killed(); // pricteme kolik jich zabijeme
            if (possibility.getKiller().getColumn() == 'a' || possibility.getKiller().getColumn() == 'h' ||
                    possibility.getKiller().getRow() == 1 || possibility.getKiller().getRow() == this.dimension)
                best_move_new--;
            if (possibility.getPosition().getColumn() == 'a' || possibility.getPosition().getColumn() == 'h' ||
                    possibility.getPosition().getRow() == 1 || possibility.getPosition().getRow() == this.dimension) {
                best_move_new++;
            }

            if (best_move_new == best_move_old) { // stejne ohodnoceny pohyb, pridame
                best.add(new Possibility(possibility));
            }
            else if (best_move_new > best_move_old) { // nasli jsme lepe ohodnoceny pohyb
                best_move_old = best_move_new;
                best = new Vector();
                best.add(new Possibility(possibility));
            }
            best_move_new = 0;
        }
        Possibility best_move;
        if (best.size() == 1)
            best_move = (Possibility)best.get(0);
        else {
            Random rand = new Random();
            int randomNum = rand.nextInt(best.size() + 1);
            if(randomNum >= best.size())
                randomNum = best.size() - 1;
            best_move = (Possibility)best.get(randomNum);
        }
        Vector to_return = new Vector();
        to_return.add(best_move);
        to_return.add(best_move_old);
        return to_return;
    }


    /* DEBUG */

    /**
     * Vlastni funkce pro vypis aktualniho stavu sachovnice
     */
    public void debug(){
        System.out.print("#|");

        for(char column = 'a'; column < (char)('a' + this.dimension); column++)
            System.out.print(column + "|");

        System.out.print("\n-+-+-+-+-+-+-+-+-+\n");

        int rows = this.dimension;
        for(int row = this.dimension; row >= 1; row--){
            System.out.print((rows--) + "|");
            for(char column = 'a'; column < (char)('a' + this.dimension); column++){
                //Zadna figurka
                if(this.at(column, row).getFigure() == null){
                    if(this.at(column, row).getColor() == Color.BLACK)
                        System.out.print("x|");
                    else
                        System.out.print(" |");
                }
                //Figurka se nasla
                else{
                    if(this.at(column, row).getFigure().getColor() == Color.BLACK)
                        System.out.print((this.at(column, row).getFigure().getClass().getSimpleName() == "Rook")? "B|" : "b|");
                    else
                        System.out.print((this.at(column, row).getFigure().getClass().getSimpleName() == "Rook")? "W|" : "w|");
                }
            }
            System.out.print("\n");
        }

        System.out.print("-+-+-+-+-+-+-+-+-+\n#|");
        for(char column = 'a'; column < (char)('a' + this.dimension); column++)
            System.out.print(column + "|");

        System.out.print("\n");
    }

}
