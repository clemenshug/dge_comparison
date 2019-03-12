library(tidyverse)
library(rvest)

l1000 <- xml2::read_html("https://www.ncbi.nlm.nih.gov/geo/query/acc.cgi?view=data&acc=GPL20573&id=80176&db=GeoDb_blob152") %>%
  rvest::html_nodes("pre") %>%
  rvest::html_text() %>%
  readr::read_tsv(comment = "#", skip = 1) %>%
  dplyr::select(entrez_gene_id = ID, gene_symbol = pr_gene_symbol)

readr::write_csv(l1000, "wrangled/l1000_gene_list.csv")
