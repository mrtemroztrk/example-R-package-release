test_that("vektorKareC works correctly", {
  expect_equal(vektorKareC(c(1, 2, 3)), c(1, 4, 9))
})

test_that("hareketliOrtalamaC works correctly", {
  x <- c(1, 2, 3, 4, 5)
  res <- hareketliOrtalamaC(x, 3)
  expect_true(is.na(res[1]))
  expect_true(is.na(res[2]))
  expect_equal(res[3:5], c(2, 3, 4))
})

test_that("grupOzetC works correctly", {
  x <- c(10, 20, 30, 40)
  g <- c("A", "A", "B", "B")
  res <- grupOzetC(x, g)
  
  idx_a <- which(res$grup == "A")
  idx_b <- which(res$grup == "B")
  
  expect_equal(res$ortalama[idx_a], 15)
  expect_equal(res$ortalama[idx_b], 35)
})

test_that("paralelUzaklikC works correctly", {
  mat <- matrix(c(0, 0, 3, 4), nrow = 2, byrow = TRUE)
  res <- paralelUzaklikC(mat, threads = 2)
  expect_equal(res[1, 2], 5)
  expect_equal(res[2, 1], 5)
  expect_equal(res[1, 1], 0)
})
