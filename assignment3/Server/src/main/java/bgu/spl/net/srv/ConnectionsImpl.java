package bgu.spl.net.srv;


import bgu.spl.net.impl.stomp.User;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class ConnectionsImpl<T> implements Connections<T> {

    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connectionHandlers = new ConcurrentHashMap<>(); // key- client connectionId, value- his connectionHandler
    private ConcurrentHashMap<String, LinkedList<Integer>> channelList = new ConcurrentHashMap<>(); // key- genre name, value- list of clients id's subscribed to this genre
    private HashMap<Integer, User> usersById = new HashMap<>();
    private final AtomicInteger id = new AtomicInteger(0);
    private final ReadWriteLock lock = new ReentrantReadWriteLock();

    @Override
    public boolean send(int connectionId, T msg) {
        if (connectionHandlers.containsKey(connectionId)) {
            connectionHandlers.get(connectionId).send(msg);
            return true;
        }
        return false;
    }

    //send frame to all the clients subscribed to channel
    @Override
    public void send(String channel, T msg) {
        if (!channelList.get(channel).isEmpty()) {
            for (Integer i : channelList.get(channel)) {
                connectionHandlers.get(i).send(msg);
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        connectionHandlers.remove(connectionId);
        //todo : maybe should remove from the history
    }

    @Override
    public int add(ConnectionHandler<T> handler) {
        int curID = id.getAndIncrement();
        lock.writeLock().lock();// Nobody can read or write at this point
        connectionHandlers.put(curID, handler);
        lock.writeLock().unlock();//Now they can

        return curID;
    }

    public void addToChannel(String channel, int userID) {
        if (!channelList.containsKey(channel)) {
            LinkedList<Integer> list = new LinkedList<>();
            channelList.put(channel, list);
        }
        channelList.get(channel).add(userID);
    }

    public void removeFromChannel(String channel, int userID) {
        if(channelList.containsKey(channel)) {
            channelList.get(channel).remove(userID);
        }
    }

}