package bgu.spl.mics.application.passiveObjects;

import java.util.*;

/**
 * Passive data-object representing a information about an agent in MI6.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You may add ONLY private fields and methods to this class.
 */
public class Squad {

    private Map<String, Agent> agents;

    //Singleton
    private static class SquadHolder {
        private static Squad instance = new Squad();
    }

    public Squad() {
        agents = new HashMap<>();
    }

    /**
     * Retrieves the single instance of this class.
     */
    public static Squad getInstance() {
        return SquadHolder.instance;
    }

    /**
     * Initializes the squad. This method adds all the agents to the squad.
     * <p>
     *
     * @param agents Data structure containing all data necessary for initialization
     *               of the squad.
     */
    public void load(Agent[] agents) {
        String sNumber;
        for (Agent agent : agents) {
            sNumber = agent.getSerialNumber();
            this.agents.put(sNumber, agent);
        }
    }

    /**
     * Releases agents.
     */
    public void releaseAgents(List<String> serials) {
        for (String sNumber : serials) {
            if (agents.get(sNumber) != null) {
                agents.get(sNumber).release();
                synchronized (agents.get(sNumber)) {
                    agents.get(sNumber).notifyAll();
                }
            }
        }
    }

    /**
     * simulates executing a mission by calling sleep.
     *
     * @param time time ticks to sleep
     */
    public void sendAgents(ArrayList<String> serials, int time) {
        if(time>0) {
            try {
                Thread.sleep(time * 100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        releaseAgents(serials);
    }

    /**
     * acquires an agent, i.e. holds the agent until the caller is done with it
     *
     * @param serials the serial numbers of the agents
     * @return ‘false’ if an agent of serialNumber ‘serial’ is missing, and ‘true’ otherwise
     */
    public boolean getAgents(List<String> serials) {
        if (!agents.keySet().containsAll(serials))
            return false;

        serials.sort(String::compareTo);
        try {
            for (String s : serials) {
                synchronized (agents.get(s)) {
                    while (!agents.get(s).isAvailable())
                        agents.get(s).wait();
                    agents.get(s).acquire();
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return true;
    }

    /**
     * gets the agents names
     *
     * @param serials the serial numbers of the agents
     * @return a list of the names of the agents with the specified serials.
     */
    public List<String> getAgentsNames(List<String> serials) {
        List<String> names = new LinkedList<>();
        String toAdd;
        for (String serial : serials) {
            toAdd = agents.get(serial).getName();
            names.add(toAdd);
        }
        return names;
    }
}
