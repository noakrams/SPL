package bgu.spl.net.impl.stomp;

import java.util.HashSet;
import java.util.Set;

public class Users extends Lock {
    private final Set<User> users = new HashSet<>();

    public User getUser(String name) {
        if (name == null) return null;
        User output = null;
        lockReader();
        for (User user : users) {
            if (user.getName().equals(name))
                output = user;
        }
        lockReaderRelease();
        return output;
    }

    public User getUser(int id) {
        User output = null;
        lockReader();//#
        for (User user : users) {
            if (user.getConnectionId() == id)
                output = user;
        }
        lockReaderRelease();//#
        return output;
    }


    boolean addIfAbsent(int id, String name, String password) {
        lockWriter();//#
        if (getUser(name) != null) {
            getUser(name).setID(id);
            return false;
        }
        User user = new User(id,name,password);
        boolean output = users.add(user);
        lockWriterRelease();//#

        return output;
    }

    public void unRegister (int id)
    {
        users.removeIf(user -> user.getConnectionId() == id);
    }


}
