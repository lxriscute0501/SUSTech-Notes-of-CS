import java.util.*;

public class GroupSystem {
    public static Student[] check(Student[] students, Group[] groups) {
        ArrayList<Student> invalidStudents = new ArrayList<>();
        boolean[] invalid = new boolean[students.length];

        for (int i = 0; i < students.length; i++)
            invalid[i] = false;

        // one-student group && two students not the same lab
        for (Group group : groups)
        {
            Student s1 = group.getStudent1();
            Student s2 = group.getStudent2();

            if (s2 != null && group.checkSameLab()) {
                markStudent(students, invalid, s1, true);
                markStudent(students, invalid, s2, true);
            }
        }

        // one appear in different groups
        for (Group g1 : groups)
        {
            for (Group g2 : groups)
            {
                if (g1 == g2) continue;
                if (sameStudent(g1, g2)) {
                    markStudent(students, invalid, g1.getStudent1(), false);
                    markStudent(students, invalid, g2.getStudent1(), false);
                    if (g1.getStudent2() != null) markStudent(students, invalid, g1.getStudent2(), false);
                    if (g2.getStudent2() != null) markStudent(students, invalid, g2.getStudent2(), false);
                }
            }
        }

        for (int i = 0; i < students.length; i++)
        {
            if (!invalid[i]) invalidStudents.add(students[i]);
        }

        if (invalidStudents.isEmpty()) return null;
        else return invalidStudents.toArray(new Student[0]);
    }

    private static void markStudent(Student[] students, boolean[] invalid, Student s, boolean value) {
        for (int i = 0; i < students.length; i++)
        {
            if (students[i] == s) {
                invalid[i] = value;
            }
        }
    }

    private static boolean sameStudent(Group g1, Group g2) {
        if (g1.getStudent1() == g2.getStudent1()) return true;
        if (g1.getStudent2() != null && g1.getStudent2() == g2.getStudent1()) return true;
        if (g2.getStudent2() != null && g1.getStudent1() == g2.getStudent2()) return true;
        if (g1.getStudent2() != null && g2.getStudent2() != null && g1.getStudent2() == g2.getStudent2()) return true;
        return false;
    }

    public static Group[] group(Student[] students) {
        ArrayList<Student>[] labStudents = new ArrayList[26];
        for (int i = 0; i < 26; i++)
            labStudents[i] = new ArrayList<>();

        for (Student s : students)
            labStudents[s.getLab() - 'A'].add(s);

        ArrayList<Group> ans = new ArrayList<>();
        for (int i = 0; i < 26; i++)
        {
            if (labStudents[i].isEmpty()) continue;

            Student[] inLabStudents = labStudents[i].toArray(new Student[0]);
            for (int j = 0; j < inLabStudents.length - 1; j++)
            {
                for (int k = j + 1; k < inLabStudents.length; k++)
                {
                    Student temp;
                    if (inLabStudents[j].getGrade() < inLabStudents[k].getGrade()) {
                        temp = inLabStudents[j];
                        inLabStudents[j] = inLabStudents[k];
                        inLabStudents[k] = temp;
                    }
                }
            }

            char lab = (char)(i + 'A');
            for (int j = 0; j < inLabStudents.length; j += 2)
            {
                if (j + 1 < inLabStudents.length) ans.add(new Group(inLabStudents[j], inLabStudents[j + 1], lab));
                else ans.add(new Group(inLabStudents[j], lab));
            }
        }

        return ans.toArray(new Group[0]);
    }
}