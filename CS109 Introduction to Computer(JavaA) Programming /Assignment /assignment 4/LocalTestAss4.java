import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Arrays;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

public class LocalTest {

    @Test
    void testFieldAndConstructor() {
        Class<?> studentClazz = Student.class;
        Class<?> groupClazz = Group.class;

        // Test Student fields
        assertField(studentClazz, "studentNumber", String.class);
        assertField(studentClazz, "lab", char.class);
        assertField(studentClazz, "grade", int.class);
//        assertAll("Student fields",
//                () -> assertField(studentClazz, "studentNumber", String.class),
//                () -> assertField(studentClazz, "lab", char.class),
//                () -> assertField(studentClazz, "grade", int.class)
//        );

        // Test Group fields
        assertField(groupClazz, "student1", Student.class);
        assertField(groupClazz, "student2", Student.class);
        assertField(groupClazz, "lab", char.class);
//        assertAll("Group Fields",
//                () -> assertField(groupClazz, "student1", Student.class),
//                () -> assertField(groupClazz, "student2", Student.class),
//                () -> assertField(groupClazz, "lab", char.class)
//        );

        // Test Student and Group constructors
        assertConstructor(Student.class, String.class, char.class, int.class);
        assertConstructor(Group.class, Student.class, char.class);
    }

    @Test
    void testToStringAndCheckSameLab() {
        Student s1 = new Student("114514", 'X', 90);
        Student s2 = new Student("1919810", 'T', 98);
        assertEquals("114514 X 90", s1.toString(), "Returned string of Student.toString() are not as expected");
        assertEquals("1919810 T 98", s2.toString(), "Returned string of Student.toString() are not as expected");
        Group groupWithOneStudent = new Group(s1, 'X');
        assertEquals("X_114514_null", groupWithOneStudent.toString(), "Returned string of Group.toString() are not as expected (Group with only one student)");
        Group groupWithTwoStudents = new Group(s1, s2, 'C');
        assertEquals("C_114514_1919810", groupWithTwoStudents.toString(), "Returned string of Group.toString() are not as expected (Group with two students)");

        s1 = new Student("114514", 'A', 90);
        s2 = new Student("1919810", 'A', 91);
        Group groupWithTwoStudentsInSameLab = new Group(s1, s2, 'A');
        assertFalse(groupWithOneStudent.checkSameLab(), "Group.checkSameLab() returns wrong value when group has only one student");
        assertFalse(groupWithTwoStudents.checkSameLab(), "Group.checkSameLab() returns wrong value when group has two students in different labs");
        assertTrue(groupWithTwoStudentsInSameLab.checkSameLab(), "Group.checkSameLab() returns wrong value when group has two students in same labs");
    }

    // GroupSystem.check() tests
    @Test
    void testCheckOverlappingStudentInGroups() {
        Student s1 = new Student("12410100", 'A', 90);
        Student s2 = new Student("12410101", 'B', 84);
        Student s3 = new Student("12410102", 'C', 90);
        Group g1 = new Group(s1, s2, 'A');
        Group g2 = new Group(s2, s3, 'B');
        Student[] students = {s1, s2, s3};
        Group[] groups = {g1, g2};
        Student[] result = GroupSystem.check(students, groups);
        assertArrayEqualsInAnyOrder(new Student[]{s2, s1, s3}, result);
    }

    @Test
    void testCheckSingleMemberGroupAndUngrouped() {
        Student s1 = new Student("12410100", 'A', 90);
        Student s2 = new Student("12410101", 'A', 84);
        Student s3 = new Student("12410102", 'A', 90);
        Group g1 = new Group(s1, 'A');
        Group[] groups = {g1};
        Student[] students = {s1, s2, s3};
        Student[] result = GroupSystem.check(students, groups);
        assertArrayEqualsInAnyOrder(new Student[]{s2, s1, s3}, result);
    }

    @Test
    void testCheckMismatchedLabGroup() {
        Student s1 = new Student("12410100", 'A', 90);
        Student s2 = new Student("12410101", 'A', 84);
        Group group = new Group(s1, s2, 'B');
        Student[] students = {s1, s2};
        Group[] groups = {group};
        Student[] result = GroupSystem.check(students, groups);
        assertArrayEqualsInAnyOrder(new Student[]{s2, s1}, result);
    }

    @Test
    void testCheckValidGroupAndNoUngrouped() {
        Student s1 = new Student("12410100", 'A', 90);
        Student s2 = new Student("12410101", 'A', 84);
        Student s3 = new Student("12410102", 'B', 90);
        Student s4 = new Student("12410103", 'B', 84);
        Group g1 = new Group(s1, s2, 'A');
        Group g2 = new Group(s3, s4, 'B');
        Student[] students = {s1, s2, s3, s4};
        Group[] groups = {g1, g2};
        Student[] result = GroupSystem.check(students, groups);
        assertNull(result, "check result is not null");
    }

    // GroupSystem.group()
    @Test
    void testGroupSingeLabEvenStudents() {
        Student[] students = createStudents(
                new String[]{"12410101", "12410102", "12410104", "12410103"},
                new char[]{'A', 'A', 'A', 'A'},
                new int[]{85, 90, 75, 80}
        );
        Group[] groups = GroupSystem.group(students);

        String[] groupsString = Arrays.stream(groups).map(Group::toString).toArray(String[]::new);
        assertArrayEqualsInAnyOrder(new String[]{"A_12410103_12410104", "A_12410101_12410102"}, groupsString);
    }

    @Test
    void testGroupSingleLabOddStudents() {
        Student[] students = createStudents(
                new String[]{"12410102", "12410101", "12410103"},
                new char[]{'B', 'B', 'B'},
                new int[]{85, 90, 80}
        );
        Group[] groups = GroupSystem.group(students);

        String[] groupsString = Arrays.stream(groups).map(Group::toString).toArray(String[]::new);
        assertArrayEqualsInAnyOrder(new String[]{"B_12410103_null", "B_12410101_12410102"}, groupsString);
        assertEquals(2, groups.length);
    }

    @Test
    void testGroupMultipleLabs() {
        Student[] students = createStudents(
                new String[]{"12410101", "12410102", "12410103", "12410104"},
                new char[]{'A', 'A', 'B', 'B'},
                new int[]{90, 85, 80, 75}
        );
        Group[] groups = GroupSystem.group(students);

        String[] groupsString = Arrays.stream(groups).map(Group::toString).toArray(String[]::new);
        assertArrayEqualsInAnyOrder(new String[]{"A_12410101_12410102", "B_12410103_12410104"}, groupsString);
    }

    @Test
    void testGroupFullRangeLabs() {
        Student[] students = {
                new Student("12410101", 'A', 90),
                new Student("12410102", 'C', 85),
                new Student("12410103", 'Z', 80),
                new Student("12410104", 'M', 75)
        };
        Group[] groups = GroupSystem.group(students);
        String[] groupsString = Arrays.stream(groups).map(Group::toString).toArray(String[]::new);
        assertArrayEqualsInAnyOrder(new String[]{"A_12410101_null", "C_12410102_null", "Z_12410103_null", "M_12410104_null"}, groupsString);
    }

    // utils
    private void assertField(Class<?> clazz, String fieldName, Class<?> expectedType) {
        Field field = assertDoesNotThrow(() -> clazz.getDeclaredField(fieldName), String.format("Field [%s] doesn't exist", fieldName));
        assertEquals(expectedType, field.getType(), String.format("Field [%s] type are not %s", fieldName, expectedType.getSimpleName()));
        assertTrue(Modifier.isPrivate(field.getModifiers()), String.format("Field [%s] is not private", fieldName));
    }

    private void assertConstructor(Class<?> clazz, Class<?>... parameterTypes) {
        String paramStr = String.format("Constructor with params [%s] doesn't exist", Arrays.stream(parameterTypes).map(Class::getSimpleName).collect(Collectors.joining(",")));
        Constructor<?> constructor = assertDoesNotThrow(() -> clazz.getDeclaredConstructor(parameterTypes), paramStr);
        assertTrue(Modifier.isPublic(constructor.getModifiers()), String.format("Constructor with params [%s] is not public", paramStr));
    }

    private void assertArrayEqualsInAnyOrder(Object[] expected, Object[] actual) {
        assertNotNull(actual, "Array is null");
        Arrays.stream(actual).forEach(element -> assertNotNull(element, "Array has null value"));
        assertEquals(expected.length, actual.length, "Array length doesn't match");
        if (!Arrays.asList(actual).containsAll(Arrays.asList(expected))) {
            Assertions.fail(String.format(
                    "Array elements mismatch (order ignored)\nExpected: %s\nActual: %s",
                    Arrays.toString(expected),
                    Arrays.toString(actual)
            ));
        }
        if (!Arrays.asList(expected).containsAll(Arrays.asList(actual))) {
            Assertions.fail(String.format(
                    "Array elements mismatch (order ignored)\nExpected: %s\nActual: %s",
                    Arrays.toString(expected),
                    Arrays.toString(actual)
            ));
        }
    }

    private Student[] createStudents(String[] numbers, char[] labs, int[] grades) {
        Student[] students = new Student[numbers.length];
        for (int i = 0; i < numbers.length; i++) {
            students[i] = new Student(numbers[i], labs[i], grades[i]);
        }
        return students;
    }

}
