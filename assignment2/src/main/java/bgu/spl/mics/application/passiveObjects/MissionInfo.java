package bgu.spl.mics.application.passiveObjects;

import java.util.ArrayList;

/**
 * Passive data-object representing information about a mission.
 * You must not alter any of the given public methods of this class.
 * <p>
 * You may add ONLY private fields and methods to this class.
 */
public class MissionInfo {

    private String name;
    private ArrayList<String> serialAgentsNumbers;
    private String gadget;
    private int timeIssued;
    private int timeExpired;
    private int duration;

    public MissionInfo(String name, ArrayList<String> serialAgentsNumbers, String gadget, int timeIssued, int timeExpired, int duration) {
        this.name = name;
        this.serialAgentsNumbers = serialAgentsNumbers;
        this.gadget = gadget;
        this.timeIssued = timeIssued;
        this.timeExpired = timeExpired;
        this.duration = duration;
    }

    /**
     * Sets the name of the mission.
     */
    public void setMissionName(String missionName) {
        this.name = missionName;
    }

    /**
     * Retrieves the name of the mission.
     */
    public String getMissionName() {
        return name;
    }

    /**
     * Sets the serial agent number.
     */
    public void setSerialAgentsNumbers(ArrayList<String> serialAgentsNumbers) {
        this.serialAgentsNumbers = serialAgentsNumbers;
    }

    /**
     * Retrieves the serial agent number.
     */
    public ArrayList<String> getSerialAgentsNumbers() {
        return serialAgentsNumbers;
    }

    /**
     * Sets the gadget name.
     */
    public void setGadget(String gadget) {
        this.gadget = gadget;
    }

    /**
     * Retrieves the gadget name.
     */
    public String getGadget() {
        return gadget;
    }

    /**
     * Sets the time the mission was issued in time ticks.
     */
    public void setTimeIssued(int timeIssued) {
        this.timeIssued = timeIssued;
    }

    /**
     * Retrieves the time the mission was issued in time ticks.
     */
    public int getTimeIssued() {
        return timeIssued;
    }

    /**
     * Sets the time that if it that time passed the mission should be aborted.
     */
    public void setTimeExpired(int timeExpired) {
        this.timeExpired = timeExpired;
    }

    /**
     * Retrieves the time that if it that time passed the mission should be aborted.
     */
    public int getTimeExpired() {
        return timeExpired;
    }

    /**
     * Sets the duration of the mission in time-ticks.
     */
    public void setDuration(int duration) {
        this.duration = duration;
    }

    /**
     * Retrieves the duration of the mission in time-ticks.
     */
    public int getDuration() {
        return duration;
    }
}
