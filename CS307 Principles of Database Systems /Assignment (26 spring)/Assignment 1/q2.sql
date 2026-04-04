SELECT TO_CHAR(
        100.0 * COUNT(*) / (SELECT COUNT(*) FROM Station), 'FM999990.00'
    ) || '%' as percentage
FROM Station
WHERE district in ('Nanshan', 'Futian', 'Luohu');