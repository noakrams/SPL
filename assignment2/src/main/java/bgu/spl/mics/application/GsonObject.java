package bgu.spl.mics.application;

import bgu.spl.mics.application.passiveObjects.Agent;
import bgu.spl.mics.application.passiveObjects.MissionInfo;
import java.util.ArrayList;


public class GsonObject {
    public String[] inventory;
    public Services services;
    public Agent[] squad;

    public class Services {
        public int M;
        public int Moneypenny;
        public Intelligence[] intelligence;
        public int time;

        public class Intelligence {
            public ArrayList<MissionInfo> missions;
            public ArrayList<MissionInfo> getMissions() {
                return missions;
            }
        }
    }
    public class Squad{
        public String name;
    }
}
