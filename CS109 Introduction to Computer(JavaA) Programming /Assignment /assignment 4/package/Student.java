public class Student {
    private String studentNumber;
    private char lab;
    private int grade;

    public Student(String studentNumber, char lab, int grade) {
        this.studentNumber = studentNumber;
        this.lab = lab;
        this.grade = grade;
    }

    public String getStudentNumber() {
        return studentNumber;
    }

    public char getLab() {
        return lab;
    }

    public int getGrade() {
        return grade;
    }

    public void setStudentNumber(String studentNumber) {
        this.studentNumber = studentNumber;
    }

    public void setLab(char lab) {
        this.lab = lab;
    }

    public void setGrade(int grade) {
        this.grade = grade;
    }

    public String toString() {
        return studentNumber + " " + lab + " " + grade;
    }
}