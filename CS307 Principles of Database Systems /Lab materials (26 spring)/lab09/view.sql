create table author
(
    id           serial primary key,
    name         varchar(20),
    age          int,
    phone_number varchar(12) unique
);

insert into author(name, age, phone_number)
VALUES ('谢佳帅', 16, '19920156699');
insert into author(name, age, phone_number)
VALUES ('徐议贤', 29, '13439473903');
insert into author(name, age, phone_number)
VALUES ('杨静雯', 29, '18771534871');
insert into author(name, age, phone_number)
VALUES ('白净瓶', 55, '19953707455');
insert into author(name, age, phone_number)
VALUES ('Caesar,Roberts', 20, '17691436667');
insert into author(name, age, phone_number)
VALUES ('Karen,Taylor', 16, '18373228462');
insert into author(name, age, phone_number)
VALUES ('靓靓', 57, '15510060852');
insert into author(name, age, phone_number)
VALUES ('whole_plan', 49, '19976169188');
insert into author(name, age, phone_number)
VALUES ('金渐层', 62, '13553629812');
insert into author(name, age, phone_number)
VALUES ('宇宙飞船', 58, '13599833785');
insert into author(name, age, phone_number)
VALUES ('Tang如风', 62, '13223508625');
insert into author(name, age, phone_number)
VALUES ('爱_walk', 46, '18656686388');
insert into author(name, age, phone_number)
VALUES ('Mee', 31, '17149966691');



create table posts
(
    post_id        serial primary key,
    title          text    not null,
    content        text,
    author_id      integer not null,
    posting_time   timestamp,
    posting_city   varchar,
    posting_nation varchar,
    constraint fk_author foreign key (author_id) references author (id),
    constraint uk unique (author_id, posting_time)
);

insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('The Benefits of Learning a Second Language',
        'Learning a second language is beneficial for many reasons. It can improve brain function, increase job opportunities, and open up a whole new world of communication and culture.',
        8, '2025-01-05 06:36:11', 'Suzhou', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('I Tried a New Recipe Last Night and It Was Delicious',
        'Last night, I decided to try a new recipe that I found online. It was a pasta dish with a creamy tomato sauce and it turned out to be absolutely delicious!',
        12, '2026-04-29 17:09:43', 'Suzhou', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('How to Improve Your Memory',
        'Having a good memory is important in many aspects of life. Here are some tips to help improve your memory:',
        10, '2020-12-15 11:01:58', 'Dallas', 'United States');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Trip to Paris',
        'Last summer, I went to Paris with my family. It was a dream come true. The city is so beautiful, and the food is amazing!',
        5, '2024-04-18 03:36:15', 'Philadelphia', 'United States');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Experience Hiking in the Mountains',
        'Last weekend, I went hiking in the mountains with some friends. It was an amazing experience to be surrounded by nature and breathe fresh air. We hiked for around 8 hours and reached the top of the mountain, where we had a breathtaking view of the scenery.',
        3, '2020-09-03 08:56:52', 'Harbin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Solo Trip Abroad: A Journey to Discover Myself',
        'Last summer, I went on my first solo trip abroad to Japan. It was a life-changing experience that allowed me to discover new things about myself.',
        1, '2019-10-01 19:28:48', 'Tianjin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Time Hiking a Mountain, It Was an Amazing Experience',
        'Last weekend, I decided to challenge myself and go hiking for the first time. It was a great experience, and I would like to share it with you all. I chose a nearby mountain that I had never been to before, and the trail was not too difficult. Along the way, I saw beautiful scenery and met friendly people. When I reached the top, I felt a sense of accomplishment that I had never felt before.',
        3, '2024-01-05 07:06:09', 'Harbin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('The Benefits of Yoga for Physical and Mental Health',
        'Practicing yoga has numerous benefits for both physical and mental health. It can improve flexibility, strength, balance, and posture, as well as reduce stress, anxiety, and depression. Furthermore, yoga is accessible to people of all ages and fitness levels, and it can be done almost anywhere with little equipment. Do you practice yoga? If so, what benefits have you experienced?',
        2, '2025-09-02 04:42:45', 'Shanghai', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Trip to Japan',
        'I recently went on a trip to Japan and it was amazing! The food, the culture, and the people were all incredible.',
        6, '2025-03-17 07:37:01', 'Shenzhen', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Recent Trip to Japan',
        'I just came back from a trip to Japan and it was amazing! The culture, food, and people were all incredible.',
        12, '2024-09-16 17:17:49', 'Mumbai', 'India');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Time Trying Out Skydiving',
        'Yesterday, I went skydiving for the first time and it was one of the most exhilarating experiences of my life. The feeling of freefalling from 14,000 feet in the air was indescribable.',
        5, '2020-09-25 18:23:05', 'Shenzhen', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('I Went to a Concert Last Night and It Was Amazing!',
        'Last night, I went to see my favorite band in concert and it was absolutely incredible. The energy was electric and I had a blast singing along to all of their songs.',
        8, '2024-12-14 03:31:24', 'Harbin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Trip to the Beach Last Weekend',
        'Last weekend, I went to the beach with my family. It was such a refreshing and relaxing trip. We spent most of our time swimming, building sandcastles, and sunbathing. The water was a bit cold, but it was still fun to swim in.',
        11, '2025-07-09 16:43:35', 'Shenzhen', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Experience Learning to Play Tennis',
        'I recently started learning how to play tennis and it has been an interesting experience so far. I never thought I would enjoy playing a sport so much.',
        11, '2022-06-12 15:22:37', 'Harbin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('A Beautiful Day for Hiking in the Mountains',
        'Today I went on a hike in the mountains and it was absolutely beautiful. The weather was perfect and the scenery was breathtaking. I feel so refreshed and rejuvenated after spending the day in nature.',
        5, '2026-02-04 03:40:41', 'Tokyo', 'Japan');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Time Traveling Solo',
        'Last week, I went on my first solo trip to Europe. It was such an adventure, and I learned so much about myself and the world around me.',
        10, '2024-04-11 04:23:20', 'Ahmedabad', 'India');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Experience Running My First Marathon',
        'Last weekend, I completed my first marathon. It was an incredible experience and something I had been training for months. The feeling of crossing the finish line was indescribable. ',
        9, '2023-02-21 07:09:53', 'Osaka', 'Japan');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Time Trying Surfing, It Was Amazing!',
        'I went to the beach for the first time and decided to try surfing. At first, I was a bit nervous, but once I caught my first wave, it was an amazing feeling. The rush of the water beneath my board and the wind in my hair was exhilarating.',
        12, '2023-02-16 16:32:17', 'Tianjin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Trip to Japan',
        'I recently went on a trip to Japan and it was such an incredible experience. The food was amazing, the people were so friendly, and the culture was so rich and fascinating.',
        12, '2025-06-17 09:57:30', 'Tianjin', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My Recent Trip to the Beach',
        'Last weekend, I went on a trip to the beach with some friends. It was amazing to escape the hustle and bustle of the city and enjoy the beautiful scenery of the coast. We spent the day swimming, sunbathing, and building sandcastles. It was so relaxing and refreshing.',
        7, '2023-01-26 19:02:12', 'Xian', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Hiking Experience Was Amazing!',
        'Yesterday, I went on my first hiking trip and it was such an amazing experience. I had never been in such close proximity to nature before and it was truly awe-inspiring. The view from the top of the mountain was breathtaking and I felt like I was on top of the world.',
        7, '2021-04-06 20:58:20', 'Xian', 'China');
insert into posts(title, content, author_id, posting_time, posting_city, posting_nation)
VALUES ('My First Time Learning to Surf',
        'I went to the beach yesterday and tried surfing for the first time. It was both exciting and challenging.', 8,
        '2024-06-19 21:37:29', 'Chicago', 'United States');
