package edu.sustech.cs307.logicalOperator;

import net.sf.jsqlparser.expression.Expression;

import java.util.Arrays;
import java.util.Collection;


public class LogicalJoinOperator extends LogicalOperator {
    private final Collection<Expression> onExpressions;
    private final LogicalOperator leftInput;
    private final LogicalOperator rightInput;

    public LogicalJoinOperator(LogicalOperator left, LogicalOperator right,
            Collection<Expression> onExpr,
            int depth) {
        super(Arrays.asList(left, right));
        this.leftInput = left;
        this.rightInput = right;
        this.onExpressions = onExpr;
    }

    public LogicalOperator getLeftInput() {
        return leftInput;
    }

    public LogicalOperator getRightInput() {
        return rightInput;
    }

    public Collection<Expression> getJoinExprs() {
        return onExpressions; // 类型转换
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        String nodeHeader = "LogicalJoinOperator(condition=" + onExpressions + ")";
        String[] leftLines = leftInput.toString().split("\\R");
        String[] rightLines = rightInput.toString().split("\\R");

        // 当前节点
        sb.append(nodeHeader);

        // 左子树处理
        if (leftLines.length > 0) {
            sb.append("\n├── ").append(leftLines[0]);
            for (int i = 1; i < leftLines.length; i++) {
                sb.append("\n│   ").append(leftLines[i]);
            }
        }

        // 右子树处理
        if (rightLines.length > 0) {
            sb.append("\n└── ").append(rightLines[0]);
            for (int i = 1; i < rightLines.length; i++) {
                sb.append("\n    ").append(rightLines[i]);
            }
        }

        return sb.toString();
    }
}
