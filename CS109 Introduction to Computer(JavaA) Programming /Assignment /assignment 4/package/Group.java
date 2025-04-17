public class Group {
    private Student student1;
    private Student student2;
    private char lab;

    public Group(Student student1, Student student2, char lab) {
        this.student1 = student1;
        this.student2 = student2;
        this.lab = lab;
    }

    public Group(Student student1, char lab) {
        this.student1 = student1;
        this.lab = lab;
        this.student2 = null;
    }

    public Student getStudent1() {
        return student1;
    }

    public Student getStudent2() {
        return student2;
    }

    public char getLab() {
        return lab;
    }

    public void setStudent1(Student student1) {
        this.student1 = student1;
    }

    public void setStudent2(Student student2) {
        this.student2 = student2;
    }

    public void setLab(char lab) {
        this.lab = lab;
    }

    public boolean checkSameLab() {
        return student1 != null && student2 != null && student1.getLab() == student2.getLab() && student1.getLab() == this.lab;
    }

    public String toString() {
        if (student2 == null) return lab + "_" + student1.getStudentNumber() + "_null";
        else {
            String s1 = student1.getStudentNumber();
            String s2 = student2.getStudentNumber();
            if (s1.compareTo(s2) < 0) return lab + "_" + s1 + "_" + s2;
            else return lab + "_" + s2 + "_" + s1;
        }
    }
}