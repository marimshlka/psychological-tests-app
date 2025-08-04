#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

struct Question {
    std::string text; // текст питання
    std::vector<std::string> options; // варіанти відповідей
    std::string imagePath; // шлях до зображення
    sf::Texture imageTexture; // текстура для зображення
    sf::Sprite imageSprite; // спрайт для відображення зображення
};

class Test {
public:
    std::string title;
    std::string filename;
};

struct TestResult {
    std::string testTitle;
    int totalWeight;
    int percentage;
};

class Quiz {
public:
    std::vector<Question> questions; // список питань тесту

    size_t current = 0; // вага відповіді, використовується для підрахунку результату
    int totalWeight = 0;

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        questions.clear(); // для повторного виклику
        std::string line;
        while (std::getline(file, line)) {
            Question q;
            q.text = line;
            for (int i = 0; i < 4; ++i) {
                std::getline(file, line);
                q.options.push_back(line);
            }

            // завантажити зображення
            std::getline(file, line);
            q.imagePath = line;
            if (!q.imageTexture.loadFromFile(q.imagePath)) {
                std::cerr << "Error loading image: " << q.imagePath << std::endl;
            } else {
                q.imageSprite.setTexture(q.imageTexture); // створюємо спрайт з текстури
            }
            questions.push_back(q);
        }
        return true;
    }
};

void saveResultsToFile(const TestResult& result, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open()) {
        // записуємо заголовок тесту
        size_t titleLength = result.testTitle.size();
        outFile.write(reinterpret_cast<const char*>(&titleLength), sizeof(titleLength));
        outFile.write(result.testTitle.c_str(), titleLength);

        // записуємо вагу результату та відсотки
        outFile.write(reinterpret_cast<const char*>(&result.totalWeight), sizeof(result.totalWeight));
        outFile.write(reinterpret_cast<const char*>(&result.percentage), sizeof(result.percentage));
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

TestResult loadResultsFromFile(const std::string& filename) {
    TestResult result;
    std::ifstream inFile(filename, std::ios::binary);
    if (inFile.is_open()) {
        size_t titleLength;
        inFile.read(reinterpret_cast<char*>(&titleLength), sizeof(titleLength));

        result.testTitle.resize(titleLength);
        inFile.read(&result.testTitle[0], titleLength);

        inFile.read(reinterpret_cast<char*>(&result.totalWeight), sizeof(result.totalWeight));
        inFile.read(reinterpret_cast<char*>(&result.percentage), sizeof(result.percentage));
        inFile.close();
    } else {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    return result;
}

std::vector<Test> loadTestList(const std::string& menuFile) {
    std::vector<Test> tests;
    std::ifstream file(menuFile);
    if (!file.is_open()) return tests;

    std::string line;
    while (std::getline(file, line)) {
        size_t sep = line.find(';'); // індекс першого знайденого символу ;
        if (sep != std::string::npos) {
            Test t;
            t.title = line.substr(0, sep); // 1 частина до символу ;
            t.filename = line.substr(sep + 1); // 2 частина після ;
            tests.push_back(t);
        }
    }
    return tests;
}

std::string getInterpretation(int percent, const std::string& testTitle) {
    if (testTitle.find("Introversion") != std::string::npos) {
        if (percent >= 80) return "You are strongly introverted.";
        if (percent >= 60) return "You tend toward introversion.";
        if (percent >= 40) return "You are balanced between introvert and extrovert.";
        if (percent >= 20) return "You tend toward extroversion.";
        return "You are highly extroverted.";
    } else if (testTitle.find("Tactile") != std::string::npos) {
        if (percent >= 80) return "You are highly sensitive to touch.";
        if (percent >= 60) return "You have moderate tactile sensitivity.";
        if (percent >= 40) return "You are moderately insensitive to touch.";
        if (percent >= 20) return "You have low tactile awareness.";
        return "You show very little tactile response.";
    } else if (testTitle.find("Emotional") != std::string::npos) {
        if (percent >= 80) return "You have excellent emotional awareness.";
        if (percent >= 60) return "You have good emotional understanding.";
        if (percent >= 40) return "Your emotional intelligence is average.";
        if (percent >= 20) return "You have some emotional difficulties.";
        return "You struggle to understand emotions.";
    } else if (testTitle.find("Cognitive") != std::string::npos) {
        if (percent >= 80) return "You are highly cognitively flexible.";
        if (percent >= 60) return "You adapt to change well.";
        if (percent >= 40) return "You have average adaptability.";
        if (percent >= 20) return "You struggle with flexibility.";
        return "You resist changes.";
    } else if (testTitle.find("Stress") != std::string::npos) {
        if (percent >= 80) return "You are very calm under stress.";
        if (percent >= 60) return "You handle stress fairly well.";
        if (percent >= 40) return "You have moderate stress levels.";
        if (percent >= 20) return "You are often stressed.";
        return "You experience high stress.";
    } else if (testTitle.find("Personality") != std::string::npos) {
        if (percent >= 80) return "You have a clear personality type.";
        if (percent >= 60) return "Your personality shows strong traits.";
        if (percent >= 40) return "You have a balanced personality.";
        if (percent >= 20) return "Your personality type is unclear.";
        return "You show mixed or weak personality traits.";
    } else if (testTitle.find("Attention") != std::string::npos) {
        if (percent >= 80) return "You have excellent attention span.";
        if (percent >= 60) return "Your focus is above average.";
        if (percent >= 40) return "You have an average attention span.";
        if (percent >= 20) return "You are easily distracted.";
        return "You struggle to concentrate.";
    } else if (testTitle.find("Empathy") != std::string::npos) {
        if (percent >= 80) return "You have high empathy.";
        if (percent >= 60) return "You are empathetic.";
        if (percent >= 40) return "You show average empathy.";
        if (percent >= 20) return "You struggle with empathy.";
        return "You show very little empathy.";
    } else if (testTitle.find("Decision") != std::string::npos) {
        if (percent >= 80) return "You are a confident decision-maker.";
        if (percent >= 60) return "You make good decisions.";
        if (percent >= 40) return "You are average in decision-making.";
        if (percent >= 20) return "You tend to hesitate or second-guess.";
        return "You struggle to make decisions.";
    } else if (testTitle.find("Anxiety") != std::string::npos) {
        if (percent >= 80) return "You show very low social anxiety.";
        if (percent >= 60) return "You handle social situations well.";
        if (percent >= 40) return "You experience moderate social anxiety.";
        if (percent >= 20) return "You are often socially anxious.";
        return "You experience high social anxiety.";
    } else {
        if (percent >= 85) return "Excellent result.";
        if (percent >= 70) return "Good understanding.";
        if (percent >= 50) return "Average performance.";
        if (percent >= 30) return "Below average.";
        return "Needs improvement.";
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 700), "Psychological Test App");
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cerr << "Font load error" << std::endl;
        return -1;
    }

    std::vector<Test> testList = loadTestList("menu.txt");
    int selectedTestIndex = -1;

    Quiz quiz;
    std::vector<int> answers;
    sf:: Text instructionsButton, gotItButton, gotItButtonsecond, gotItButtonthird; // додано для кнопки Instructions
    sf::Text questionText, resultText, retryText, prevButton, nextButton, warningText, backToMenuButton;
    sf::Text confirmText, yesButton, cancelButton, BinaryTestResult, ButtonPreviousBinaryResult;
    bool inQuiz, quizFinished, showWarning, showExitConfirmation = false;
    bool showBinaryResultFromPreviousTest, showTestConfirmation, showInstructionswindow, showBinaryTestResult = false;
    bool showInstructions = true; // прапор для показу інструкцій

    std::string instructionText = "Welcome to the Psychological Test App!\n\n" // інструкція для відображення
    "1. Select a test from the main menu.\n"
    "2. Answer the multiple-choice questions.\n"
    "3. After completing the test, view your results.\n\n"
    "Click 'Got it!' to return to the main menu.";

    std::string BinaryTestResultText, BinaryResultText;
    sf::Text titleText;
    std::vector<sf::Text> menuItems(10);
    std::vector<sf::Text> answerItems(4);

    // створити індикатор виконання для тесту
    sf::RectangleShape progressBarBackground(sf::Vector2f(400, 20));
    progressBarBackground.setFillColor(sf::Color(220, 220, 220));
    progressBarBackground.setPosition(100, 650); // нижня частина вікна

    sf::RectangleShape progressBar(sf::Vector2f(0, 20));
    progressBar.setFillColor(sf::Color(0, 128, 255));
    progressBar.setPosition(100, 650);

    sf::Text progressText;
    progressText.setFont(font);
    progressText.setCharacterSize(20);
    progressText.setFillColor(sf::Color::Black);
    progressText.setPosition(510, 640);

    std::vector<std::string> testReviews = { // рецензія для вибраного тесту
        "Measures your introversion based on preferences.",
        "Evaluates your sensitivity to tactile stimuli.",
        "Assesses emotional intelligence, including and empathy.",
        "Evaluates your adaptability to problem-solving.",
        "Gauges stress levels based on stress-related scenarios.",
        "Evaluates personality type based on Carl Jung's theory.",
        "Measures your attention span and focus during tasks.",
        "Assesses your ability to empathize with others emotionally.",
        "Evaluates your decision-making process.",
        "Identifies how much social anxiety you experience."
    };

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event; // інформація про події в програмі
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!inQuiz && event.type == sf::Event::MouseButtonPressed) {
                if (showTestConfirmation) {
                    if (yesButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // розпочати тест, якщо натиснуто Yes
                        quiz.loadFromFile(testList[selectedTestIndex].filename);
                        answers = std::vector<int>(quiz.questions.size(), -1);
                        inQuiz = true;
                        quizFinished = false;
                        quiz.current = 0;
                        quiz.totalWeight = 0;
                        showTestConfirmation = false;
                    } else if (cancelButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        // повернутися до меню, якщо натиснути Back to menu
                        showTestConfirmation = false;
                    }
                    continue;
                }

                for (int i = 0; i < menuItems.size(); ++i) {
                    if (menuItems[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selectedTestIndex = i;
                        // показати меню підтвердження перед початком тесту
                        showTestConfirmation = true;
                    }
                }
            }

            if (inQuiz && !quizFinished && event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 4; ++i) {
                    if (answerItems[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        answers[quiz.current] = i;
                        //quiz.totalWeight += 3 - i;
                        if (i == 0) { // найкраща відповідь
                            quiz.totalWeight += 3;
                        } else if (i == 1) { // друга відповідь
                            quiz.totalWeight += 2;
                        } else if (i == 2) { // третя відповідь
                            quiz.totalWeight += 1;
                        } else if (i == 3) { // найгірший варіант
                            quiz.totalWeight += 0;
                        }

                        quiz.current++;
                        showWarning = false;
                        if (quiz.current >= quiz.questions.size())
                            quizFinished = true;
                    }
                }
            }

            if (showExitConfirmation && event.type == sf::Event::MouseButtonPressed) {
                if (yesButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    inQuiz = false;
                    quizFinished = false;
                    quiz.current = 0;
                    quiz.totalWeight = 0;
                    showExitConfirmation = false;
                } else if (cancelButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    showExitConfirmation = false;
                    inQuiz = true;
                }
            }

            if (inQuiz && !quizFinished && event.type == sf::Event::MouseButtonPressed) {
                if (backToMenuButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    showExitConfirmation = true;
                    inQuiz = false;
                }

                if (prevButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (quiz.current > 0) quiz.current--;
                } else if (nextButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (answers[quiz.current] == -1) { // якщо відповідь ще не вибрана, то за замовчування -1
                        showWarning = true;
                    } else {
                        quiz.current++;
                        showWarning = false;
                        if (quiz.current >= quiz.questions.size()) quizFinished = true;
                    }
                }
            } else if (quizFinished && retryText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    inQuiz = false;
                    quizFinished = false;
                    quiz.current = 0;
                    quiz.totalWeight = 0;
                    showWarning = false; // щоб не залишався warning після повторного запуску
                }
            }

            if (instructionsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (showInstructions && event.type == sf::Event::MouseButtonPressed) {
                    showInstructionswindow = true;
                }
            }

            if (showInstructionswindow && event.type == sf::Event::MouseButtonPressed) { // кнопка 'Got it!' для закриття інструкцій
                if (gotItButton.getGlobalBounds().contains (mousePos.x, mousePos.y)) {
                    showInstructionswindow = false; // при натисканні на Got it! закриваємо інструкцію
                }
            }

            if (BinaryTestResult.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (!showBinaryTestResult && event.type == sf::Event::MouseButtonPressed) {
                    TestResult loadedResult = loadResultsFromFile("test_results.dat");

                    BinaryTestResultText =
                        "Test: " + loadedResult.testTitle + "\n\n" +
                        "Score: " + std::to_string(loadedResult.totalWeight) + "\n" +
                        "Percentage: " + std::to_string(loadedResult.percentage) + "%\n\n" +
                        "Click 'Got it!' to close the window.";

                    showBinaryTestResult = true;
                }
            }

            if (showBinaryTestResult && event.type == sf::Event::MouseButtonPressed) { // кнопка 'Got it!' для закриття інструкцій
                if (gotItButtonsecond.getGlobalBounds().contains (mousePos.x, mousePos.y)) {
                    showBinaryTestResult = false; // при натисканні на Got it! закриваємо інструкцію
                }
            }

            if (ButtonPreviousBinaryResult.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (!showBinaryResultFromPreviousTest && event.type == sf::Event::MouseButtonPressed) {
                    TestResult loadedResult = loadResultsFromFile("test_results.dat");

                    BinaryTestResultText =
                        "Test: " + loadedResult.testTitle + "\n\n" +
                        "Score: " + std::to_string(loadedResult.totalWeight) + "\n" +
                        "Percentage: " + std::to_string(loadedResult.percentage) + "%\n\n" +
                        "Click 'Got it!' to close the window.";

                    showBinaryResultFromPreviousTest = true;
                }
            }

            if (showBinaryResultFromPreviousTest && event.type == sf::Event::MouseButtonPressed) { // кнопка 'Got it!' для закриття інструкцій
                if (gotItButtonthird.getGlobalBounds().contains (mousePos.x, mousePos.y)) {
                    showBinaryResultFromPreviousTest = false; // при натисканні на Got it! закриваємо інструкцію
                }
            }
        }

        sf::Vector2f mouseFloatPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); // конвертує цілісні координати миші в float
        window.clear(sf::Color(240, 248, 255)); // очищує вікно і встановлює фон кольору блакитного відтінку

        if (!inQuiz) {
            titleText.setFont(font);
            titleText.setString("Psyhological Test App");
            titleText.setCharacterSize(36);
            titleText.setFillColor(sf::Color::Black);
            titleText.setPosition(270, 30);
            window.draw(titleText);

            titleText.setFont(font);
            titleText.setString("Choose a test:");
            titleText.setCharacterSize(25);
            titleText.setFillColor(sf::Color::Black);
            titleText.setPosition(50, 100);
            window.draw(titleText);

            for (int i = 0; i < testList.size(); ++i) {
                menuItems[i].setFont(font);
                menuItems[i].setCharacterSize(24);
                if (i == selectedTestIndex) {
                    menuItems[i].setFillColor(sf::Color::Red); // виділити попередній вибраний тест
                } else {
                    menuItems[i].setFillColor(menuItems[i].getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Blue : sf::Color(0, 102, 204));
                    // якщо миша знаходиться на елементі меню, колір тексту змінюється на Blue, інакше звичайний колір
                }
                menuItems[i].setPosition(70, 145 + i * 50);
                menuItems[i].setString(std::to_string(i + 1) + ". " + testList[i].title);
                window.draw(menuItems[i]);
            }

            if (showInstructions) {
                instructionsButton.setFont(font);
                instructionsButton.setString("Instructions");
                instructionsButton.setCharacterSize (25);
                instructionsButton.setFillColor (instructionsButton.getGlobalBounds().contains (mousePos.x, mousePos.y) ? sf:: Color:: Red: sf:: Color(0, 128, 0));
                instructionsButton.setPosition(50, 70);
                window.draw(instructionsButton);
            }

            if (showInstructionswindow) {
                sf::RectangleShape overlay(sf::Vector2f(600, 200));
                overlay.setFillColor(sf::Color(255, 255, 255, 230));
                overlay.setOutlineColor(sf::Color::Black);
                overlay.setOutlineThickness(2);
                overlay.setPosition(150, 200);
                window.draw(overlay);

                confirmText.setFont(font);
                confirmText.setString(instructionText);
                confirmText.setCharacterSize(22);
                confirmText.setFillColor(sf::Color::Black);
                confirmText.setPosition(170, 220);
                window.draw(confirmText);

                gotItButton.setFont(font);
                gotItButton.setString("Got it!");
                gotItButton.setCharacterSize(30);
                gotItButton.setFillColor(gotItButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(128, 0, 0));
                gotItButton.setPosition(625, 275);
                window.draw(gotItButton);
            }

            if (showTestConfirmation) { // показати екран підтвердження
                sf::RectangleShape overlay(sf::Vector2f(600, 200));
                overlay.setFillColor(sf::Color(255, 255, 255, 230));
                overlay.setOutlineColor(sf::Color::Black);
                overlay.setOutlineThickness(2);
                overlay.setPosition(150, 200);
                window.draw(overlay);

                confirmText.setFont(font);
                confirmText.setString("Do you want to take this test?\n" + testReviews[selectedTestIndex]);
                confirmText.setCharacterSize(22);
                confirmText.setFillColor(sf::Color::Black);
                confirmText.setPosition(170, 220);
                window.draw(confirmText);

                yesButton.setFont(font);
                yesButton.setString("Yes");
                yesButton.setCharacterSize(20);
                yesButton.setFillColor(yesButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 128, 0));
                yesButton.setPosition(200, 270);
                window.draw(yesButton);

                cancelButton.setFont(font);
                cancelButton.setString("Back to menu");
                cancelButton.setCharacterSize(20);
                cancelButton.setFillColor(cancelButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(128, 0, 0));
                cancelButton.setPosition(300, 270);
                window.draw(cancelButton);
            }

            if (showExitConfirmation) { // показати екран підтвердження
                sf::RectangleShape overlay(sf::Vector2f(600, 200));
                overlay.setFillColor(sf::Color(255, 255, 255, 230));
                overlay.setOutlineColor(sf::Color::Black);
                overlay.setOutlineThickness(2);
                overlay.setPosition(150, 200);
                window.draw(overlay);

                confirmText.setFont(font);
                confirmText.setString("Do you want to go back to the menu?\n");
                confirmText.setCharacterSize(22);
                confirmText.setFillColor(sf::Color::Black);
                confirmText.setPosition(170, 220);
                window.draw(confirmText);

                yesButton.setFont(font);
                yesButton.setString("Yes");
                yesButton.setCharacterSize(20);
                yesButton.setFillColor(yesButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 128, 0));
                yesButton.setPosition(200, 270);
                window.draw(yesButton);

                cancelButton.setFont(font);
                cancelButton.setString("Cancel");
                cancelButton.setCharacterSize(20);
                cancelButton.setFillColor(cancelButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(128, 0, 0));
                cancelButton.setPosition(300, 270);
                window.draw(cancelButton);
            }

            ButtonPreviousBinaryResult.setFont(font);
            ButtonPreviousBinaryResult.setCharacterSize(24);
            ButtonPreviousBinaryResult.setFillColor(ButtonPreviousBinaryResult.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 128, 0));
            ButtonPreviousBinaryResult.setPosition(50, 630);
            ButtonPreviousBinaryResult.setString("Click here to open result from test_results.dat binary file");
            window.draw(ButtonPreviousBinaryResult);

            if (showBinaryResultFromPreviousTest) { // показати екран підтвердження
                sf::RectangleShape overlay(sf::Vector2f(600, 200));
                overlay.setFillColor(sf::Color(255, 255, 255, 230));
                overlay.setOutlineColor(sf::Color::Black);
                overlay.setOutlineThickness(2);
                overlay.setPosition(150, 200);
                window.draw(overlay);

                confirmText.setFont(font);
                confirmText.setString(BinaryTestResultText);
                confirmText.setCharacterSize(22);
                confirmText.setFillColor(sf::Color::Black);
                confirmText.setPosition(170, 220);
                window.draw(confirmText);

                gotItButtonthird.setFont(font);
                gotItButtonthird.setString("Got it!");
                gotItButtonthird.setCharacterSize(30);
                gotItButtonthird.setFillColor(gotItButtonthird.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(128, 0, 0));
                gotItButtonthird.setPosition(625, 275);
                window.draw(gotItButtonthird);
            }

        } else if (!quizFinished) {
            // оновити індикатор виконання
            float progress = (float)quiz.current / quiz.questions.size();
            progressBar.setSize(sf::Vector2f(400 * progress, 20));
            progressText.setString(std::to_string((int)(progress * 100)) + "%");

            prevButton.setFont(font);
            prevButton.setString("< Back");
            prevButton.setCharacterSize(20);
            prevButton.setFillColor(prevButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 102, 204));
            prevButton.setPosition(50, 380);
            window.draw(prevButton);

            backToMenuButton.setFont(font);
            backToMenuButton.setString("Back to menu");
            backToMenuButton.setCharacterSize(20);
            backToMenuButton.setFillColor(backToMenuButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 102, 204));
            backToMenuButton.setPosition(50, 420); // під кнопкою < Back
            window.draw(backToMenuButton);

            nextButton.setFont(font);
            nextButton.setString("Next >");
            nextButton.setCharacterSize(20);
            nextButton.setFillColor(nextButton.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 102, 204));
            nextButton.setPosition(750, 380);
            window.draw(nextButton);

            Question& q = quiz.questions[quiz.current];
            questionText.setFont(font);
            questionText.setString(q.text);
            questionText.setCharacterSize(26);
            questionText.setFillColor(sf::Color::Black);
            questionText.setPosition(50, 30);
            window.draw(questionText);

            // відображення зображення (якщо воно є)
            if (!q.imagePath.empty()) {
                sf::Texture testTexture;
                if (testTexture.loadFromFile("images/image" + std::to_string(selectedTestIndex + 1) + "." + std::to_string(quiz.current + 1) + ".png")) {
                    sf::Sprite testSprite;
                    // задаємо бажану ширину і висоту
                    float targetWidth = 250.f;
                    float targetHeight = 200.f;
                    float scaleX = targetWidth / testTexture.getSize().x;
                    float scaleY = targetHeight / testTexture.getSize().y;
                    testSprite.setScale(scaleX, scaleY);
                    testSprite.setTexture(testTexture);
                    testSprite.setPosition(425, 100);
                    window.draw(testSprite);
                }
            }

            if (showWarning) {
                warningText.setFont(font);
                warningText.setCharacterSize(18);
                warningText.setFillColor(sf::Color::Red);
                warningText.setString("Select an answer to move on to the next question.");
                warningText.setPosition(250, 420);
                window.draw(warningText);
            }

            for (int i = 0; i < 4; ++i) {
                answerItems[i].setFont(font);
                answerItems[i].setCharacterSize(22);
                if (answerItems[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) { // якщо наводимо мишку
                    answerItems[i].setFillColor(sf::Color(255, 165, 0)); // помаранчевий колір наведення

                    if (answers[quiz.current] == i) { // перевіряється, чи вибрав користувач цей варіант відповіді
                        answerItems[i].setFillColor(sf::Color::Red); // виділити вибрану відповідь
                    } else if (answerItems[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        answerItems[i].setFillColor(sf::Color(255, 165, 0)); // помаранчевий при наведенні курсора, та сама умова
                    } else {
                        answerItems[i].setFillColor(sf::Color(0, 102, 153)); // по замовчуванню
                    }

                } else {
                    answerItems[i].setFillColor(sf::Color(0, 102, 153));
                }
                answerItems[i].setPosition(70, 100 + i * 60);
                answerItems[i].setString(std::to_string(i + 1) + ") " + q.options[i]);
                window.draw(answerItems[i]);
            }

            window.draw(progressBarBackground);
            window.draw(progressBar);
            window.draw(progressText);

        } else if (quizFinished) {
            int maxScore = quiz.questions.size() * 3; // quiz.questions.size() — кі-сть питань у тесті. макс бал за питання - 3
            // якщо в тесті 11 питань: 11 * 3 = 33 = maxScore
            int percentage = (quiz.totalWeight * 100) / maxScore;

            TestResult result;
            result.testTitle = testList[selectedTestIndex].title;
            result.totalWeight = quiz.totalWeight;
            result.percentage = percentage;

            resultText.setFont(font);
            resultText.setCharacterSize(30);
            resultText.setFillColor(sf::Color::Black);
            resultText.setPosition(100, 50);
            resultText.setString("Your result: " + std::to_string(percentage) + "%");
            window.draw(resultText);

            // намальована діаграма (пряма) для візуального зворотнього зв'язку
            sf::RectangleShape backgroundBar(sf::Vector2f(400, 30));
            backgroundBar.setFillColor(sf::Color(220, 220, 220));
            backgroundBar.setPosition(100, 100);
            window.draw(backgroundBar);

            sf::RectangleShape resultBar(sf::Vector2f(4 * percentage, 30));
            resultBar.setFillColor(sf::Color(0, 128, 255));
            resultBar.setPosition(100, 100);
            window.draw(resultBar);

            sf::Text percentLabel;
            percentLabel.setFont(font);
            percentLabel.setCharacterSize(20);
            percentLabel.setFillColor(sf::Color::Black);
            percentLabel.setPosition(510, 100);
            percentLabel.setString(std::to_string(percentage) + "%");
            window.draw(percentLabel);

            sf::Text interpretationText;
            interpretationText.setFont(font);
            interpretationText.setCharacterSize(22);
            interpretationText.setFillColor(sf::Color::Black);
            interpretationText.setPosition(100, 140);
            interpretationText.setString(getInterpretation(percentage, testList[selectedTestIndex].title));
            window.draw(interpretationText);

            retryText.setFont(font);
            retryText.setCharacterSize(24);
            retryText.setFillColor(retryText.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 128, 0));
            retryText.setPosition(100, 160);
            retryText.setString("Click here to try another test");
            window.draw(retryText);

            BinaryTestResult.setFont(font);
            BinaryTestResult.setCharacterSize(24);
            BinaryTestResult.setFillColor(BinaryTestResult.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(0, 128, 0));
            BinaryTestResult.setPosition(100, 190);
            BinaryTestResult.setString("Click here to open result from binary file");
            window.draw(BinaryTestResult);

            // намальовані відповіді під результатом
            sf::Text answersText;
            answersText.setFont(font);
            answersText.setCharacterSize(18);
            answersText.setFillColor(sf::Color::Black);
            answersText.setPosition(100, 220);
            std::string answerDetails = "";

            // відповіді на кожне запитання
            for (size_t i = 0; i < quiz.questions.size(); ++i) {
                answerDetails += "Q" + std::to_string(i + 1) + ": " + quiz.questions[i].options[answers[i]] + "\n\n";
            }
            answersText.setString(answerDetails);
            window.draw(answersText);

            // перевірка валідності: якщо питання 8 і 11 однакові, але відповіді різні, то виводиться відповідне повідомлення
            std::string validityWarning = "";
            if (quiz.questions.size() >= 11) { // бо ми користуємось 11м питанням і воно має існувати
                const Question& q8 = quiz.questions[7];
                const Question& q11 = quiz.questions[10];

                bool textsEqual = (q8.text == q11.text);
                bool answersEqual = (answers[7] == answers[10]);

                if (textsEqual && !answersEqual) {
                    validityWarning = "Test validity compromised:\nanswers to identical questions\ndo not match as expected.";
                }
            }

            if (!validityWarning.empty()) {
                sf::Text warningText;
                warningText.setFont(font);
                warningText.setCharacterSize(20);
                warningText.setFillColor(sf::Color::Red);
                warningText.setString(validityWarning);
                warningText.setPosition(580, 65);
                window.draw(warningText);
            }

            saveResultsToFile(result, "test_results.dat"); // зберігаємо результат у бінарному файлі

            if (showBinaryTestResult) {
                sf::RectangleShape overlay(sf::Vector2f(600, 200));
                overlay.setFillColor(sf::Color(255, 255, 255, 230));
                overlay.setOutlineColor(sf::Color::Black);
                overlay.setOutlineThickness(2);
                overlay.setPosition(150, 200);
                window.draw(overlay);

                confirmText.setFont(font);
                confirmText.setString(BinaryTestResultText);
                confirmText.setCharacterSize(22);
                confirmText.setFillColor(sf::Color::Black);
                confirmText.setPosition(170, 220);
                window.draw(confirmText);

                gotItButtonsecond.setFont(font);
                gotItButtonsecond.setString("Got it!");
                gotItButtonsecond.setCharacterSize(30);
                gotItButtonsecond.setFillColor(gotItButtonsecond.getGlobalBounds().contains(mouseFloatPos) ? sf::Color::Red : sf::Color(128, 0, 0));
                gotItButtonsecond.setPosition(625, 275);
                window.draw(gotItButtonsecond);
            }
        }

        window.display();
    }

    return 0;
}