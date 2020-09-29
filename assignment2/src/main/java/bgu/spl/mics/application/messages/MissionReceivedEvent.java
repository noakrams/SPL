package bgu.spl.mics.application.messages;

import bgu.spl.mics.Event;
import bgu.spl.mics.application.passiveObjects.MissionInfo;

import java.util.ArrayList;
import java.util.List;

public class MissionReceivedEvent implements Event<Void> {
    private MissionInfo missionInfo;

    public MissionReceivedEvent(MissionInfo missionInfo) {
        this.missionInfo = missionInfo;
    }

    public MissionInfo getMissionInfo() {
        return missionInfo;
    }

    public String getName() {
        return missionInfo.getMissionName();
    }

    public ArrayList<String> getAgentsNumber() {
        return missionInfo.getSerialAgentsNumbers();
    }

    public int getDuration() {
        return missionInfo.getDuration();
    }

    public String getGadget() {
        return missionInfo.getGadget();
    }

    public int getTimeExpired() {
        return missionInfo.getTimeExpired();
    }

    public int getTimeIssued() {
        return missionInfo.getTimeIssued();
    }
}
