package bgu.spl.mics.application.passiveObjects;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Serializable;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Arrays;


/**
 * That's where Q holds his gadget (e.g. an explosive pen was used in GoldenEye, a geiger counter in Dr. No, etc).
 * <p>
 * This class must be implemented safely as a thread-safe singleton.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You can add ONLY private fields and methods to this class as you see fit.
 */
public class Inventory implements Serializable {

    private ArrayList<String> gadgets;
    private static final Object lock = new Object();

    //Singleton
    private static class InventoryHolder {
        private static Inventory instance = new Inventory();
    }

    /**
     * Retrieves the single instance of this class.
     */
    public static Inventory getInstance() {
        return InventoryHolder.instance;
    }

    public Inventory() {
        this.gadgets = new ArrayList<>();
    }


    /**
     * Initializes the inventory. This method adds all the items given to the gadget
     * inventory.
     * <p>
     *
     * @param inventory Data structure containing all data necessary for initialization
     *                  of the inventory.
     */
    public void load(String[] inventory) {
        gadgets.addAll(Arrays.asList(inventory));
    }

    /**
     * acquires a gadget and returns 'true' if it exists.
     * <p>
     *
     * @param gadget Name of the gadget to check if available
     * @return ‘false’ if the gadget is missing, and ‘true’ otherwise
     */
    public boolean getItem(String gadget) {
        synchronized (lock) {
            return gadgets.remove(gadget);
        }
    }

    /**
     * <p>
     * Prints to a file name @filename a serialized object List<String> which is a
     * list of all the of the gadgeds.
     * This method is called by the main method in order to generate the output.
     */
    public void printToFile(String filename) {
        try (Writer writer = new FileWriter(filename)) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(gadgets, writer);
        } catch (IOException ignored) {
        }
    }
}